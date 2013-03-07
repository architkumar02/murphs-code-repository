#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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

  // Creating ROOT analysis objects (histogram)
  char name[256] ="/home/tmp_scale/murffer/";
  char title[256];
  char buffer[64];

#ifdef G4MPIUSE
  char hostName[64];
  gethostname(hostName,64);
  G4int rank= G4MPImanager::GetManager()-> GetRank();
  sprintf(buffer,"run_%d_\0",aRun->GetRunID());
  strcat(name,buffer);
  strcat(name,hostName);  
  sprintf(buffer,"_%03d.root",rank);
  strcat(name,buffer);
#else
  sprintf(name,"run_%d.root",aRun->GetRunID());
#endif
  outfile = new TFile(name,"RECREATE");
  posEDepTuple = new TNtuple("posEDepTuple","Initial Position and Energy Deposition","x:y:z:EnergyDep");
  eDepHist = new TH1F("eDepHist","Total Energy Deposition",500,0*eV,5*MeV);
  G4cout<<"Prepared run "<<aRun->GetRunID()<<G4endl;
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
      if (i == 0 ){
        // First Hit is the intial interaction
        xPos = hit->GetPosition().x();
        yPos = hit->GetPosition().y();
        zPos = hit->GetPosition().z();
      }

      // Adding the energy deposition
      eDepEvent=+hit->GetEdep();
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

  G4cout<<"End Of Run "<<aRun->GetRunID()
        <<"\tAverage Energy Deposition (MeV): "<<eDepHist->GetMean()
        <<" +/- "<<eDepHist->GetMeanError()<<G4endl;
  outfile->Write();
  outfile->Close();
  delete outfile;
}
