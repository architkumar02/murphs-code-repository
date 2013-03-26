#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

#ifdef G4MPIUSE
#include "G4MPImanager.hh"
#endif

Analysis* Analysis::singleton = 0;

/**
 * Analysis
 *
 * Creates an Analysis object 
 */
Analysis::Analysis(){
  // Empty Constructor, assingment done in constuctor list
  maxHistEnergy = 5*MeV;
  posHistBinWidth = 5*um;
  incidentParticleName = "";
}
Analysis::~Analysis(){
  G4cout<<"Deleting the analysis object"<<G4endl;
}

/**
 * PrepareNewRun
 *
 * @brief - called before each run.
 * There is no need to update the geometry because the geometry
 * is fixed during a run.
 */
void Analysis::PrepareNewRun(const G4Run* aRun){

  // Getting the detector thickness
  G4double detThickness = GetDetectorThickness();
  G4String detMat = GetDetectorMaterial();

  std::ostringstream oss;
#ifdef G4MPIUSE
  char hostName[64];
  gethostname(hostName,64);
  oss <<incidentParticleName<<"run_"<<aRun->GetRunID()<<"_"
      <<hostName<<"_rank_"<<G4MPImanager::GetManager()-> GetRank()
      <<detMat<<"_"<<G4BestUnit(detThickness,"Length")<<".root";
#else
  oss <<incidentParticleName<<"_"
      <<detMat<<"_"<<G4BestUnit(detThickness,"Length")<<".root";
#endif
  std::string fname = oss.str();
  fname.erase(remove(fname.begin(), fname.end(),' '),fname.end());
  
  // Creating ROOT analysis objects (histogram)
  outfile = new TFile(fname.data(),"RECREATE");
  posEDepTuple = new TNtuple("posEDepTuple","Initial Position and Energy Deposition","x:y:z:EnergyDep");
  eDepHist = new TH1F("eDepHist","Total Energy Deposition",500,0*eV,maxHistEnergy);
  G4cout<<"Prepared run "<<aRun->GetRunID()<<G4endl;
}
/**
 * GetDetectorMaterial
 */
G4String Analysis::GetDetectorMaterial(){
  G4LogicalVolume* detLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Absorber");
  G4Material* detMat = NULL;
  if ( detLV) {
    detMat = dynamic_cast< G4Material*>(detLV->GetMaterial()); 
  } 
  if (detMat)
    return detMat->GetName();
  else
    return G4String("UNKOWN");
}
/**
 * GetDetectorThickness
 * @return the thickness of the detector, from the G4LogicalVolumeStore
 */
G4double Analysis::GetDetectorThickness(){
  G4double detThickness = 0;
  G4LogicalVolume* detLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("Absorber");
  G4Tubs* detTubs = 0;
  if ( detLV) {
    detTubs = dynamic_cast< G4Tubs*>(detLV->GetSolid()); 
  } 
  if ( detTubs ) {
    detThickness = detTubs->GetZHalfLength()*2;  
  }
  else  {
    G4cerr << "Detector Thickness not found." << G4endl;
  } 
  return detThickness;
}
/**
 * PrepareNewEvent
 *
 * @brief - Called before each event
 * The energy deposition per slice is initialzed per event
 */
void Analysis::PrepareNewEvent(const G4Event* anEvent){
  // Initialize energy deposition to zero
  eDepEvent = 0.0;
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){

  G4VHitsCollection *hc;
  G4double xPos = 0.0; 
  G4double yPos = 0.0;
  G4double zPos = 0.0; 
  CaloHit *hit;

  // Iterating through the hit collection to accumulate the energy deposition 
  G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
  for(G4int hitItter = 0; hitItter < numHitColl; hitItter++){
    // Itterating through the hit collection
    hc = event->GetHCofThisEvent()->GetHC(hitItter);
    for(G4int i = 0; i < hc->GetSize(); i++){
      hit = (CaloHit*) hc->GetHit(i);
      //hit->Print();
      if (hit->GetTrackID() == 2 && hit->GetParentID() == 1){
        // First interaction of the particle
        xPos = hit->GetPosition().x();
        yPos = hit->GetPosition().y();
        zPos = hit->GetPosition().z();
      }

      // Adding the energy deposition (in MeV)
      eDepEvent += hit->GetEdep()/MeV;
    }
  }
  // Adding to the run accumulation
  eDepHist->Fill(eDepEvent);
  posEDepTuple->Fill(xPos,yPos,zPos,eDepEvent);
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){

  // Lets get the number of events simulated
  G4int numEvents = aRun->GetNumberOfEventToBeProcessed();
  G4double detThickness = GetDetectorThickness();
  
  G4cout<<"End Of Run "<<aRun->GetRunID()
        <<"\n\tRan "<<numEvents<<" events"
        <<"\n\tThickness [mm]: "<<detThickness/mm
        <<"\n\tAverage Energy Deposition per event (MeV): "<<eDepHist->GetMean()
        <<" +/- "<<eDepHist->GetMeanError()<<G4endl;
  outfile->Write();
  outfile->Close();
  delete outfile;
}
/**
 * SetIncidentParticleName
 *
 * Sets the incident particle name (for the messener / file name)
 */
void Analysis::SetIncidentParticleName(G4String pName){
  incidentParticleName = pName;
}

/**
 * SetHistEMax
 *
 * Sets the maximum energy of the Analysis histogram 
 */
void Analysis::SetHistEMax(G4double emax){
  maxHistEnergy = emax;
}
/**
 * SetBinWidth
 *
 * Sets the width of the position 
 */
void Analysis::SetBinWidth(G4double binWidth){
  posHistBinWidth = binWidth;
}
