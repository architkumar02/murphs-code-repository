#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4ParticleDefinition.hh"
#include "TMath.h"
#include "TAxis.h"

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
  oss <<incidentParticleName<<"_"
    <<detMat<<"_"<<G4BestUnit(detThickness,"Length")<<".root";
  std::string fname = oss.str();
  fname.erase(remove(fname.begin(), fname.end(),' '),fname.end());

  // Creating ROOT analysis objects (histogram)
  outfile = new TFile(fname.data(),"RECREATE");
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
 */
void Analysis::PrepareNewEvent(const G4Event* event){}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){}
/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){
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

  if (incidentParticleName == "neutron" )
    neutron = true;
  else
    neutron = false;
}

