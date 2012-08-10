#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "TFile.h"
#include "globals.hh"

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

    G4cout<<"Preparing new event "<<anEvent->GetEventID()<<G4endl;
}

void Analysis::PrepareNewRun(const G4Run* aRun){
    thisRunTotSecondaries = 0;
    thisRunNumGammas = 0;
    thisRunNumElectrons = 0;
    for(int i = 0; i<NUMLAYERS; i++){
    //    thisRunTotEGap[i] = 0;
        thisRunTotEAbs[i] = 0;
    }

    // Preparing Histograms
    hLongProfile = new TProfile("LongProfile","Calo Long Profile",
                    NUMLAYERS,-0.5,NUMLAYERS-0.5);
    hLongProfile->GetXaxis()->SetTitle("Layer Num");
    hLongProfile->GetYaxis()->SetTitle("E_{Abs} (MeV)");

    hTotE = new TH1F("Total Energy","Calo Total Energy in Gap",100,0,500);
    hTotE->GetXaxis()->SetTitle("E_{Abs} (MeV)");
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
    G4cout<<"Summary for run: "<<aRun->GetRunID()
    <<"\n Event Processed: "<<numEvents
    <<"\n Average Number of Secondaries: "<<thisRunTotSecondaries/numEvents
    <<"\n Average Number of Gammas: "<<thisRunNumGammas/numEvents
    <<"\n Average Number of Electrons: "<<thisRunNumElectrons/numEvents<<G4endl;
    for(int i=0; i<NUMLAYERS; i++){
        G4cout<<"\n Average Energy in Layer "<<i
       // <<"\n \t Gap: "<<G4BestUnit(thisRunTotEGap[i]/numEvents,"Energy")
        <<"\n \t Abs: "<<G4BestUnit(thisRunTotEAbs[i]/numEvents,"Energy")
        <<G4endl;
    }
    G4cout<<"============="<<G4endl;
    
    // Writing Histograms
    char filename[256];
    sprintf(filename,"run_%d.root",aRun->GetRunID());
    TFile* outfile = TFile::Open(filename,"recreate");
    G4cout<<"Created file "<<filename<<G4endl;
    hLongProfile->Write();
    hTotE->Write();
    G4cout<<"Wrote file "<<filename<<G4endl;
    outfile->Close();
    G4cout<<"Closed file "<<filename<<G4endl;

    delete hLongProfile;
    delete hTotE;
}
