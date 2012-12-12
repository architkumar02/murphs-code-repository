#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4String.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "G4Trajectory.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include <fstream>

#include "TString.h"

#ifdef G4MPIUSE
#include "G4MPImanager.hh"
#endif

Analysis* Analysis::singleton = 0;

Analysis::Analysis(DetectorConstruction* det):detector(det){
    // Empty Constructor, assingment done in constuctor list
    G4cout<< "Analysis is based on a detector of "
    <<"\n -> "<<detector->GetNumberGapSlices()<<" gap slices"
    <<"\n -> "<<detector->GetNumberAbsSlices()<<" absorber slices"
    <<G4endl;
}

void Analysis::PrepareNewEvent(const G4Event* anEvent){

}

void Analysis::PrepareNewRun(const G4Run* aRun){
}

/**
 * ProcessHitCollection
 *
 * @param G4VHitsCollection *hc
 *
 * Helper method to process hit collections
 */
void Analysis::ProcessHitCollection(G4VHitsCollection *hc,G4int eventID){
}

/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){
}
/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){
}
