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

/**
 * Analysis
 *
 * Creates an Analysis object based on the geometry contained
 * in det. 
 */
Analysis::Analysis(DetectorConstruction* det):detector(det){
    // Empty Constructor, assingment done in constuctor list
    sliceThickness = 5*um;
    ComputeParameters();
}
/**
 * ComputeParameters
 *
 * Computes the parameters of this run
 */
void Analysis::ComputeParameters(){
    // Getting geometry of detector
    G4double absThickness = detector->GetAbsorberThickness();
    G4double gapThickness = detector->GetGapThickness();

    // Computing number of slices
    numberAbsSlices = ceil(absThickness/sliceThickness);
    numberGapSlices = ceil(gapThickness/sliceThickness);
    numberSlices = 2*numberGapSlices+numberAbsSlices;
}

/**
 * PrepareNewEvent
 *
 * @brief - Called before each event
 * The energy deposition per slice is initialzed per event
 */
void Analysis::PrepareNewEvent(const G4Event* anEvent){
    // Initialize energy deposition to zero
    for(G4int i = 0; i < numberSlices; i++)
        eDepEvent[i] = 0.0;
}
/**
 * PrepareNewRun
 *
 * @brief - called before each run
 */
void Analysis::PrepareNewRun(const G4Run* aRun){
   ComputeParameters();
   
   // Creating space for the energy deposition
    muEDepRun = (G4double*) malloc(sizeof(G4double)*numberSlices);
    varEDepRun = (G4double*) malloc(sizeof(G4double)*numberSlices);
    numEntries = (G4int*) malloc(sizeof(G4int)*numberSlices);
    eDepEvent = (G4double*) malloc(sizeof(G4double)*numberSlices);
    if (muEDepRun == NULL || varEDepRun == NULL || 
            numEntries == NULL || eDepEvent == NULL){
        G4Exception("Analsysis","malloc error",FatalException,"Could not allocate analysis accumulation variables");
    }

    // Initialize average energy depsoition (and variance with entries)
    for(G4int i = 0; i < numberSlices; i++){
        muEDepRun[i] = 0.0;
        varEDepRun[i] = 0.0;
        numEntries[i] = 0.0;
    }
}


/**
 * EndOfEvent
 *
 * @param G4Event* event
 */
void Analysis::EndOfEvent(const G4Event* event){
    // Need to itteratite through the event's hit collection to collect
    // the energy deposition
    G4VHitsCollection *hc;
    G4double eDep;
    G4double zPos;
    CaloHit *hit;
    G4int slice;
    G4double offset = detector->GetGapThickness()+detector->GetAbsorberThickness()/2;
    G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
    for(G4int hitItter = 0; hitItter < numHitColl; hitItter++){
        // Itterating through the hit collection
        hc = event->GetHCofThisEvent()->GetHC(hitItter);
        for(G4int i = 0; i < hc->GetSize(); i++){
            hit = (CaloHit*) hc->GetHit(i);

            // Getting Hit Properties
            eDep = hit->GetEdep();
            zPos = hit->GetPosition().z();
            
            // Getting the slice from the position (pg 53 of lab notebook)
            slice = floor( (zPos+offset)/sliceThickness); 
        
            // Adding the energy to the slice
            eDepEvent[slice]+=eDep;
        }
    }

    // Need to copy the event energy deposition total into the run total
    for(G4int i = 0; i < numberSlices; i++){
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
    for (G4int i = 0; i < numberSlices; i++){
        muEDepRun[i] = muEDepRun[i]/numEntries[i];
        varEDepRun[i] = (numEntries[i]/(numEntries[i] - 1))*(
                varEDepRun[i]/numEntries[i] - (muEDepRun[i]*muEDepRun[i]));

    }

    // Need to write out the the data
    char name[256] ="/home/tmp_scale/murffer/";

#ifdef G4MPIUSE
    char hostName[64];
    char buffer[64];
    gethostname(hostName,64);
    G4int rank= G4MPImanager::GetManager()-> GetRank();
    sprintf(buffer,"run_%d_\0",aRun->GetRunID());
    strcat(name,buffer);
    strcat(name,hostName);  
    sprintf(buffer,"_%03d.txt",rank);
    strcat(name,buffer);
#else
    sprintf(name,"run_%d.txt",aRun->GetRunID());
#endif
    FILE* outfile = fopen(name,"w");
    
    // Writing Run Information
    G4double offset = detector->GetGapThickness()+detector->GetAbsorberThickness()/2;
    G4double constant = -offset+sliceThickness/2;

    fprintf(outfile,"##################### Run Information ####################\n");
    fprintf(outfile,"# Run %d\n",aRun->GetRunID());
    fprintf(outfile,"################## Geometry Information ##################\n");
    fprintf(outfile,"# Absorber is %5.3f um (%d slices)\n",detector->GetAbsorberThickness()/um,numberAbsSlices);
    fprintf(outfile,"# Gap (one side) is %5.3f cm (%d slices)\n",detector->GetGapThickness()/cm,numberGapSlices);
    fprintf(outfile,"# A slice is %5.3f um\n",sliceThickness);
    fprintf(outfile,"# Slice(Z) = floor( (Z + %5.3e)/%5.3e)\n",offset,sliceThickness);
    fprintf(outfile,"################## Energy Deposition ##################\n");
    fprintf(outfile,"# Slice\t\tPos\tMean\tVariance\tNum Entries\n");
    for(G4int i = 0; i < numberSlices; i++)
        fprintf(outfile,"%d\t%7.5f\t%7.5f\t%7.5f\t%d\n",i,i*sliceThickness+constant,muEDepRun[i],varEDepRun[i],numEntries[i]);
    fclose(outfile);

    // Free Willy!
    free(muEDepRun);
    free(varEDepRun);
    free(numEntries);
    free(eDepEvent);
}
