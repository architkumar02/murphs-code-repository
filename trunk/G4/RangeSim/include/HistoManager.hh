#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace AIDA {
 class IAnalysisFactory;
 class ITree;
 class IHistogram1D;
}

class HistoMessenger;

const G4int MaxHisto = 4;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
  public:

    HistoManager();
   ~HistoManager();

    void SetFileName   (const G4String& name) { fileName[0] = name;};
    void SetFileType   (const G4String& name) { fileType    = name;};
    void SetFileOption (const G4String& name) { fileOption  = name;};    
    void book();
    void save();
    void SetHisto (G4int,G4int,G4double,G4double,const G4String& unit="none");  
    void FillHisto(G4int id, G4double e, G4double weight = 1.0);
    void RemoveHisto (G4int);
    void PrintHisto  (G4int);
    
    G4bool    HistoExist  (G4int id) {return exist[id];}
    G4double  GetHistoUnit(G4int id) {return Unit[id];}
    G4double  GetBinWidth (G4int id) {return Width[id];}

  private:

    G4String                 fileName[2];
    G4String                 fileType;
    G4String                 fileOption;    
    AIDA::IAnalysisFactory*  af;
    AIDA::ITree*             tree;
    AIDA::IHistogram1D*      histo[MaxHisto];
    G4bool                   exist[MaxHisto];
    G4String                 Label[MaxHisto];
    G4String                 Title[MaxHisto];
    G4int                    Nbins[MaxHisto];
    G4double                 Vmin [MaxHisto];
    G4double                 Vmax [MaxHisto];
    G4double                 Unit [MaxHisto];
    G4double                 Width[MaxHisto];
    G4bool                   ascii[MaxHisto];
        
    G4bool                   factoryOn;
    HistoMessenger*          histoMessenger;
    
  private:
    void saveAscii();            
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

