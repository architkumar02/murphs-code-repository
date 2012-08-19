#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include <string.h>

#include "G4HCofThisEvent.hh"
#include "G4Event.hh"

Analysis* Analysis::singleton = 0;

Analysis::Analysis(){
    // Empty Constructor, assingment done in constuctor list
}
/*
void Analysis::AddHit(CaloHit* hit){
    G4int layerNum = hit->GetLayerNumber();
    if (strcmp(hit->GetVolume()->GetName(),"Gap")){
        thisEventTotEGap[layerNum] += hit->GetEdep();
    }else if(strcmp(hit->GetVolume()->GetName(),"Absorber")){
        thisEventTotEAbs[layerNum] += hit->GetEdep();
    }
    else{
        G4cout<<"ERROR - Unkown Volume for sensitive detector"<<G4endl;
    }
    if (hit->GetParticleRank() > 1)
        thisEventTotNumSecondaries[layerNum]++;
}
*/

void Analysis::PrepareNewEvent(const G4Event* anEvent){
    // Nothing to be done before an event
    thisEventSecondaries = 0;
    thisEventNumGammas = 0;
    thisEventNumElectrons = 0;
    for(int i = 0; i<NUMLAYERS+1; i++){
        thisEventTotEGap[i] = 0;
        thisEventTotEAbs[i] = 0;
        thisEventTotNumSecondaries[i] = 0;
    }
}

void Analysis::PrepareNewRun(const G4Run* aRun){

    char filename[256];
    sprintf(filename,"run_%d.root",aRun->GetRunID());
    outfile = new TFile(filename,"recreate");


    // Creating NTuples
    for(int i=0; i<NUMLAYERS+1; i++){
        if (i<NUMLAYERS){
            char* name = new char[30];
            sprintf(name,"GapLayer_%d",i);
            runTupleGap[i] = new TNtuple(name,name,"stepLength:eDep:kEnergy:pID");
        }else{
            runTupleGap[i] = new TNtuple("GapTotal","GapTotal","stepLength:eDep:kEnergy:pID");
        }
    }

    for(int i=0; i<NUMLAYERS+1; i++){
        if (i<NUMLAYERS){
            char* name = new char[30];
            sprintf(name,"AbsLayer_%d",i);
            runTupleAbs[i] = new TNtuple(name,name,"stepLength:eDep:kEnergy:pID");
        }else{
            runTupleAbs[i] = new TNtuple("AbsTotal","AbsTotal","stepLength:eDep:kEnergy:pID");
        }
    }
    for(int i=0; i<NUMLAYERS+1; i++){
        if (i<NUMLAYERS){
            char* name = new char[30];
            sprintf(name,"Layer_%d",i);
            runTupleLayer[i] = new TNtuple(name,name,"stepLength:eDep:kEnergy:pID");
        }else{
            runTupleLayer[i] = new TNtuple("CaloTotal","CaloTotal","stepLength:eDep:kEnergy:pID");
        }
    }
}

void Analysis::EndOfEvent(const G4Event* event){

    // Looping through the hit collection
    G4VHitsCollection *hc = event->GetHCofThisEvent()->GetHC(0);
    for(int i = 0; i < hc->GetSize(); i++){
        hc->GetHit(i)->Print();
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
