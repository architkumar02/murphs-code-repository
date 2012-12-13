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

Analysis::Analysis(DetectorConstruction* det):detector(det){
    // Empty Constructor, assingment done in constuctor list
    G4cout<< "Analysis is based on a detector of "
    <<"\n -> "<<detector->GetNumberGapSlices()<<" gap slices"
    <<"\n -> "<<detector->GetNumberAbsSlices()<<" absorber slices"
    <<G4endl;
    numSlices = detector->GetNumberGapSlices() + detector->GetNumberAbsSlices();
}

/**
 * PrepareNewEvent
 *
 * Called before each event
 */
void Analysis::PrepareNewEvent(const G4Event* anEvent){
    // Initialize energy deposition to zero
    for(G4int i = 0; i < numSlices; i++)
        eDepEvent[i] = 0.0;
}

void Analysis::PrepareNewRun(const G4Run* aRun){
    // Creating space for the energy deposition
    muEDepRun = (G4double*) malloc(sizeof(G4double)*numSlices);
    varEDepRun = (G4double*) malloc(sizeof(G4double)*numSlices);
    numEntries = (G4int*) malloc(sizeof(G4int)*numSlices);
    eDepEvent = (G4double*) malloc(sizeof(G4double)*numSlices);
    if (muEDepRun == NULL || varEDepRun == NULL || 
        numEntries == NULL || eDepEvent == NULL){
        G4Exception("Analsysis","malloc error",FatalException,"Could not allocate analysis accumulation variables");
    }

    // Initialize average energy depsoition (and variance with entries)
    for(G4int i = 0; i < numSlices; i++){
        muEDepRun[i] = 0.0;
        varEDepRun[i] = 0.0;
        numEntries[i] = 0.0;
    }
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
    // Need to copy the event energy deposition total into the run total
    for(G4int i = 0; i < numSlices; i++){
        numEntries[i] ++;
        muEDepRun[i] += eDepEvent[i];
        varEDepRun[i] += eDepEvent[i]*eDepEvent[i];
    }
}
/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){
    // Calculting the statistics!
    for (G4int i = 0; i < numSlices; i++){
        muEDepRun[i] = muEDepRun[i]/numEntries[i];
        varEDepRun[i] = (numEntries[i]/(numEntries[i] - 1))*(
            varEDepRun[i]/numEntries[i] - (muEDepRun[i]*muEDepRun[i]));

    }

    // Need to write out the the data
    G4cout<<"Need to write out the data!"<<G4endl;
    
    // Free Willy!
    free(muEDepRun);
    free(varEDepRun);
    free(numEntries);
    free(eDepEvent);
}
