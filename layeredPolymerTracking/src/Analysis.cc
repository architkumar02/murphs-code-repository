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

Analysis::Analysis(){
    // Empty Constructor, assingment done in constuctor list
}

void Analysis::PrepareNewEvent(const G4Event* anEvent){

    // Resetting Accumulation Varaibles
    for(int i= 0; i < NUMLAYERS+1; i++){
        eventEDepTot_Abs[i] = 0.0;
        eventEDepTot_Gap[i] = 0.0;
    }
}

void Analysis::PrepareNewRun(const G4Run* aRun){

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

    G4double binMin = 0*eV;
    G4double binMax = 5*MeV;
    G4int numBins = 2000;
    for(int i = 0; i < NUMLAYERS; i++){
        /*  Hit Histograms */
        sprintf(name,"totHitEDepAbs_%02i",i);
        sprintf(title,"Total Energy Deposited in a Hit (Abs Layer %2i)",i);
        hHitTotEDepAbs[i] = new TH1F(name,title,numBins,binMin,binMax);

        sprintf(name,"totHitEDepGap_%02i",i);
        sprintf(title,"Total Energy Deposited in a Hit (Gap Layer %2i)",i);
        hHitTotEDepGap[i] = new TH1F(name,title,numBins,binMin,binMax);

        /* Event Histograms */
        sprintf(name,"totEventEDepGap_%02i",i);
        sprintf(title,"Total Energy Deposited in an Event (Gap Layer %2i)",i);
        hEventTotEDepGap[i] = new TH1F(name,title,numBins,binMin,binMax);

        sprintf(name,"totEventEDepAbs_%02i",i);
        sprintf(title,"Total Energy Deposited in an Event (Abs Layer %2i)",i);
        hEventTotEDepAbs[i] = new TH1F(name,title,numBins,binMin,binMax);

        /* Distrubtion of Secondary Electrons Kinetic Energy */
        for (int j = 0; j < NUMPID; j++){
            sprintf(name,"secElectronKinE_%02i_%02iPID",i,j);
            sprintf(title,"Kinetic Energy of First Secondary Electron (Abs Layer %2i, PID=%2i)",i,j);
            hSecElecKinAbs[i][j] = new TH1F(name,title,numBins,binMin,binMax);
        }
    }
    /* Event Histogram (All Layers) */
    hEventTotEDepGap[NUMLAYERS] = new TH1F("totEventEDepGap",
            "Total Energy Deposited in an Event (All Gap Layers)",numBins,binMin,binMax);
    hEventTotEDepAbs[NUMLAYERS] = new TH1F("totEventEDepAbs",
            "Total Energy Deposited in an Event (All Abs Layers)",numBins,binMin,binMax);
    hEventTotCalo = new TH1F("totEventEDepCalo",
            "Total Energy Deposited in Calorimeter",numBins,binMin,binMax);
}

void Analysis::EndOfEvent(const G4Event* event){

    // Processing all of the hit collections
    // (Fills the hit histograms)
    G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
    for(G4int i = 0; i < numHitColl; i++){
        ProcessHitCollection( event->GetHCofThisEvent()->GetHC(i),event->GetEventID());
    }
    // Filling event enery histograms
    for(int i= 0; i < NUMLAYERS+1; i++){
        (hEventTotEDepAbs[i])->Fill(eventEDepTot_Abs[i]);     // Abs
        (hEventTotEDepGap[i])->Fill(eventEDepTot_Gap[i]);     // Gap
    }
    // Event total for calorimeter (Abs+Gap)
    hEventTotCalo->Fill(eventEDepTot_Abs[NUMLAYERS]+eventEDepTot_Gap[NUMLAYERS]); 
}

/**
 * ProcessHitCollection
 *
 * @param G4VHitsCollection *hc
 *
 * Helper method to process hit collections
 */
void Analysis::ProcessHitCollection(G4VHitsCollection *hc,G4int eventID){


    // Looping through the hit collection
    G4double hitColEDepTot_Abs[NUMLAYERS+1];   // Total EDep (abs) for Hit Collection
    G4double hitColEDepTot_Gap[NUMLAYERS+1];   // Total EDep (gap) for Hit Collection
    for(int i= 0; i < NUMLAYERS+1; i++){
        hitColEDepTot_Abs[i] = 0.0;
        hitColEDepTot_Gap[i] = 0.0;
    }

    G4int secElectronTrackID[NUMLAYERS][NUMPID];
    G4double secElectronKinE[NUMLAYERS][NUMPID];
    G4int PID;
    for (int i = 0; i < NUMLAYERS; i++){
        for (int j = 0; j <NUMPID; j++){
            secElectronTrackID[i][j] = 0;
            secElectronKinE[i][j] = 0;
        }
    }

    // Energy Deposition of the event
    for(G4int i = 0; i < hc->GetSize(); i++){
        CaloHit* hit = (CaloHit*) hc->GetHit(i);

        G4double eDep = hit->GetEdep();
        G4int layerNum = hit->GetLayerNumber();
        if (strcmp(hit->GetVolume()->GetName(),"Gap")){
            // Hit occured in the Gap
            hitColEDepTot_Gap[layerNum] += eDep;
            (hHitTotEDepGap[layerNum])->Fill(eDep);

        }else if(strcmp(hit->GetVolume()->GetName(),"Absorber")){
            // Hit occured in the Abs
            hitColEDepTot_Abs[layerNum] += eDep;
            (hHitTotEDepAbs[layerNum])->Fill(eDep);

            /* Is this the first secondary electron of the event? */
            if(hit->GetParticle()->GetPDGEncoding() == 11){
                PID = hit->GetParentID();

                /**
                 * PID == 1  (first secondary particle) 
                 * PID == 2 (second secondary particle) 
                 * PID == 3 (third secondary particle) 
                 **/
                if (PID < NUMPID){
                    /* The first secondary with have the highest track id */
                    if (hit->GetTrackID() > secElectronTrackID[layerNum][PID]){
                        secElectronTrackID[layerNum][PID] = hit->GetTrackID();
                        secElectronKinE[layerNum][PID] = hit->GetKineticEnergy();
                    }
                }
            }
        }
        else{
            G4cout<<"ERROR - Unkown Volume for sensitive detector"<<G4endl;
        }
    }

    /* Filling Secondary Electron Energy */
    for (int i = 0; i < NUMLAYERS; i++){
        for(int j = 0; j < NUMPID; j++){
            (hSecElecKinAbs[i][j])->Fill(secElectronKinE[i][j]);
        }
    }

    // Adding this Hit collection's energy deposited to event total
    for (int i = 0; i < NUMLAYERS; i++){
        // Incrementing each individual bin
        eventEDepTot_Abs[i] += hitColEDepTot_Abs[i];
        eventEDepTot_Gap[i] += hitColEDepTot_Gap[i];

        // Last bin is Calorimter Total (all Abs layers and all Gap layers)
        eventEDepTot_Abs[NUMLAYERS] += hitColEDepTot_Abs[i];
        eventEDepTot_Gap[NUMLAYERS] += hitColEDepTot_Gap[i];
    }
}

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
