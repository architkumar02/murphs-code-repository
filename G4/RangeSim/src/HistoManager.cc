#include "HistoManager.hh"
#include "HistoMessenger.hh"
#include "G4UnitsTable.hh"

#ifdef G4ANALYSIS_USE
#include "AIDA/AIDA.h"
#endif


HistoManager::HistoManager()
:af(0),tree(0),factoryOn(false)
{
#ifdef G4ANALYSIS_USE
  // Creating the analysis factory
  af = AIDA_createAnalysisFactory();
  if(!af) {
    G4cout << " HistoManager::HistoManager() :" 
           << " problem creating the AIDA analysis factory."
           << G4endl;
  }
#endif 
 
  fileName[0] = "rangesim1";
  fileType    = "root";
  fileOption  = "";
  // histograms
  for (G4int k=0; k<MaxHisto; k++) {
    histo[k] = 0;
    exist[k] = false;
    Unit[k]  = 1.0;
    Width[k] = 1.0;
    ascii[k] = false;    
  }

  histoMessenger = new HistoMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete histoMessenger;
  
#ifdef G4ANALYSIS_USE  
  delete af;
#endif  
}

void HistoManager::book()
{
#ifdef G4ANALYSIS_USE
  if(!af) return;

  // Creating a tree mapped to an hbook file.
  fileName[1] = fileName[0] + "." + fileType;
  G4bool readOnly  = false;
  G4bool createNew = true;
  AIDA::ITreeFactory* tf  = af->createTreeFactory();
  tree = tf->create(fileName[1], fileType, readOnly, createNew, fileOption);
  delete tf;
  if(!tree) {
    G4cout << "HistoManager::book() :" 
           << " problem creating the AIDA tree with "
           << " storeName = " << fileName[1]
           << " storeType = " << fileType
           << " readOnly = "  << readOnly
           << " createNew = " << createNew
           << " options = "   << fileOption
           << G4endl;
    return;
  }

  // Creating a histogram factory, whose histograms will be handled by the tree
  AIDA::IHistogramFactory* hf = af->createHistogramFactory(*tree);

  // create selected histograms
  for (G4int k=0; k<MaxHisto; k++) {
    if (exist[k]) {
      histo[k] = hf->createHistogram1D( Label[k], Title[k],
                                                  Nbins[k], Vmin[k], Vmax[k]);
      factoryOn = true;
    }
  }
  delete hf;
  if (factoryOn) 
     G4cout << "\n----> Histogram Tree is opened in " << fileName[1] << G4endl;
#endif
}

void HistoManager::save()
{
#ifdef G4ANALYSIS_USE
  if (factoryOn) {
    saveAscii();          // Write ascii file, if any   
    tree->commit();       // Writing the histograms to the file
    tree->close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved in " << fileName[1] << G4endl;

    delete tree;
    tree = 0;
    factoryOn = false;
  }
#endif
}


void HistoManager::FillHisto(G4int ih, G4double e, G4double weight)
{
  if (ih > MaxHisto) {
    G4cout << "---> warning from HistoManager::FillHisto() : histo " << ih
           << "does not exist; e= " << e << " w= " << weight << G4endl;
    return;
  }
#ifdef G4ANALYSIS_USE
  if(exist[ih]) histo[ih]->fill(e/Unit[ih], weight);
#endif
}


void HistoManager::SetHisto(G4int ih,
                 G4int nbins, G4double valmin, G4double valmax, const G4String& unit)
{
  if (ih > MaxHisto) {
    G4cout << "---> warning from HistoManager::SetHisto() : histo " << ih
           << "does not exist" << G4endl;
    return;
  }
  
  const G4String id[] = { "0", "1", "2", "3" };
  const G4String title[] = 
                { "dummy",					//0
                  "total track length of primary particle",	//1
                  "nb steps of primary particle",		//2
                  "step size of primary particle"		//3
                 };


  G4String titl = title[ih];
  G4double vmin = valmin, vmax = valmax;
  Unit[ih] = 1.;

  if (unit != "none") {
    titl = title[ih] + " (" + unit + ")";
    Unit[ih] = G4UnitDefinition::GetValueOf(unit);
    vmin = valmin/Unit[ih]; vmax = valmax/Unit[ih];
  }

  exist[ih] = true;
  Label[ih] = id[ih];
  Title[ih] = titl;
  Nbins[ih] = nbins;
  Vmin[ih]  = vmin;
  Vmax[ih]  = vmax;
  Width[ih] = (valmax-valmin)/nbins;

  G4cout << "----> SetHisto " << ih << ": " << titl << ";  "
         << nbins << " bins from "
         << vmin << " " << unit << " to " << vmax << " " << unit << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::RemoveHisto(G4int ih)
{
 if (ih > MaxHisto) {
    G4cout << "---> warning from HistoManager::RemoveHisto() : histo " << ih
           << "does not exist" << G4endl;
    return;
  }

  histo[ih] = 0;  exist[ih] = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintHisto(G4int ih)
{
 if (ih < MaxHisto) { ascii[ih] = true; ascii[0] = true; }
 else
    G4cout << "---> warning from HistoManager::PrintHisto() : histo " << ih
           << "does not exist" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <fstream>

void HistoManager::saveAscii()
{
#ifdef G4ANALYSIS_USE

 if (!ascii[0]) return;
 
 G4String name = fileName[0] + ".ascii";
 std::ofstream File(name, std::ios::out);
 File.setf( std::ios::scientific, std::ios::floatfield );
 
 //write selected histograms
 for (G4int ih=0; ih<MaxHisto; ih++) {
    if (exist[ih] && ascii[ih]) {
      File << "\n  1D histogram " << ih << ": " << Title[ih] 
           << "\n \n \t     X \t\t     Y" << G4endl;
     
      for (G4int iBin=0; iBin<Nbins[ih]; iBin++) {
         File << "  " << iBin << "\t" 
	      << 0.5*(histo[ih]->axis().binLowerEdge(iBin) +
	              histo[ih]->axis().binUpperEdge(iBin)) << "\t"
	      << histo[ih]->binHeight(iBin) 
	      << G4endl;
      } 
    }
 }
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

