#include "Analysis.hh"
#include "G4UnitsTable.hh"

Analysis* Analysis::singleton = 0;

Analysis::Analysis() : 
    thisEventSecondaries(0),
    thisRunTotSecondaries(0),
    thisEventNumGammas(0),
    thisRunNumGammas(0),
    thisEventNumElectrons(0),
    thisRunNumElectrons(0){
    // Empty Constructor, assingment done in constuctor list
}

void Analysis::PrepareNewEvent(const G4Event* anEvent){
    thisEventSecondaries = 0;
    thisEventNumGammas = 0;
    thisEventNumElectrons = 0;
    for(int i = 0; i<NUMLAYERS; i++){
      //  thisEventTotEGap[i] = 0;
        thisEventTotEAbs[i] = 0;
    }
}

void Analysis::PrepareNewRun(const G4Run* aRun){
    thisRunTotSecondaries = 0;
    thisRunNumGammas = 0;
    thisRunNumElectrons = 0;
    for(int i = 0; i<NUMLAYERS; i++){
    //    thisRunTotEGap[i] = 0;
        thisRunTotEAbs[i] = 0;
    }
}

void Analysis::EndOfEvent(const G4Event* anEvent){
    thisRunTotSecondaries += thisEventSecondaries;
    thisRunNumGammas += thisEventNumGammas;
    thisRunNumElectrons += thisEventNumElectrons;
    for(int i = 0; i < NUMLAYERS; i++){
     //   thisRunTotEGap[i] += thisEventTotEGap[i];
        thisRunTotEAbs[i] += thisEventTotEAbs[i];
    }
}

void Analysis::EndOfRun(const G4Run* aRun){
    G4int numEvents = aRun->GetNumberOfEvent();

    G4cout<<"============="<<G4endl;
    G4cout<<"Summary for run: ">>aRun->GetRunID()
    <<"\n Event Processed: "<<numEvents
    <<"\n Average Number of Secondaries: "<<thisRunTotSecondaries/numEvents
    <<"\n Average Number of Gammas: "<<thisRunNumGammas/numEvents
    <<"\n Average Number of Electrons: "<<thisRunNumElectrons/numEvents<<G4endl;
    for(int i=0; i<NUMLAYERS; i++){
        <<"\n Average Energy in Layer "<<i
       // <<"\n \t Gap: "<<G4BestUnit(thisRunTotEGap[i]/numEvents,"Energy")
        <<"\n \t Abs: "<<G4BestUnit(thisRunTotEAbs[i]/numEvents,"Energy")
    }
    G4cout<<"============="<<G4endl;
}
