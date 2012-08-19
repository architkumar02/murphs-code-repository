#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include <string.h>

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


void Analysis::PrepareNewEvent(const G4Event* anEvent){
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
	thisRunTotSecondaries = 0;
	thisRunNumGammas = 0;
	thisRunNumElectrons = 0;
	for(int i = 0; i<NUMLAYERS+1; i++){
		thisRunTotEGap[i] = 0;
		thisRunTotEAbs[i] = 0;
		thisRunTotNumSecondaries[i] = 0;
	}

	char filename[256];
	sprintf(filename,"run_%d.root",aRun->GetRunID());
	outfile = new TFile(filename,"recreate");

	// Preparing Histograms
	hProfileEGap = new TProfile("LongEGap","Calo EGap Profile",
			NUMLAYERS,-0.5,NUMLAYERS-0.5);
	hProfileEGap->GetXaxis()->SetTitle("Layer Num");
	hProfileEGap->GetYaxis()->SetTitle("E_{Gap} (keV)");

	hProfileEAbs = new TProfile("LongEAbs","Calo EAbs Profile",
			NUMLAYERS,-0.5,NUMLAYERS-0.5);
	hProfileEAbs->GetXaxis()->SetTitle("Layer Num");
	hProfileEAbs->GetYaxis()->SetTitle("E_{Abs} (keV)");

	hProfileNumSec = new TProfile("LongNumSec","Calo NumSec Profile",
			NUMLAYERS,-0.5,NUMLAYERS-0.5);
	hProfileNumSec->GetXaxis()->SetTitle("Layer Num");
	hProfileNumSec->GetYaxis()->SetTitle("Number Secondaries");

    G4double maxEnergy = 1010;      // in Kev
    G4double minEnergy = 0.01;      // in Kev
    G4int nBins = 100;
	hTotEGap = new TH1F("TotEGap","Total Energy in Gap",
                nBins,minEnergy,maxEnergy);
	hTotEGap->GetXaxis()->SetTitle("E_{Gap} (keV)");
	hTotEAbs = new TH1F("TotEAbs","Total Energy in Abs",
                nBins,minEnergy,maxEnergy);
	hTotEAbs->GetXaxis()->SetTitle("E_{Abs} (keV)");

	for(int i=0; i<NUMLAYERS; i++){
		char* histName = new char[30];
		sprintf(histName,"h_EGapLayer_%d",i);
		hEDepGapLayer[i] = new TH1F(histName,"",nBins,minEnergy,maxEnergy);
	}
	for(int i=0; i<NUMLAYERS; i++){
		char* histName = new char[30];
		sprintf(histName,"h_EAbsLayer_%d",i);
		hEDepAbsLayer[i] = new TH1F(histName,"",nBins,minEnergy,maxEnergy);
	}
}

void Analysis::EndOfEvent(const G4Event* anEvent){
	thisRunTotSecondaries += thisEventSecondaries;
	thisRunNumGammas += thisEventNumGammas;
	thisRunNumElectrons += thisEventNumElectrons;

	for(int i = 0; i < NUMLAYERS; i++){
		thisRunTotEGap[i] += thisEventTotEGap[i];
		thisRunTotEAbs[i] += thisEventTotEAbs[i];

		thisEventTotEGap[NUMLAYERS] += thisEventTotEGap[i];
		thisEventTotEAbs[NUMLAYERS] += thisEventTotEAbs[i];

		thisRunTotNumSecondaries[NUMLAYERS] += thisEventTotNumSecondaries[i];

		// Filling Histograms
		hEDepGapLayer[i]->Fill(thisEventTotEGap[i]/keV);
		hEDepAbsLayer[i]->Fill(thisEventTotEAbs[i]/keV);
	}
	thisRunTotEGap[NUMLAYERS] += thisEventTotEGap[NUMLAYERS];
	thisRunTotEAbs[NUMLAYERS] += thisEventTotEAbs[NUMLAYERS];
	hTotEGap->Fill(thisEventTotEGap[NUMLAYERS]);
	hTotEAbs->Fill(thisEventTotEAbs[NUMLAYERS]);

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
			<<"\n \t Gap: "<<G4BestUnit(thisRunTotEGap[i]/numEvents,"Energy")
			<<"\n \t Abs: "<<G4BestUnit(thisRunTotEAbs[i]/numEvents,"Energy")
			<<G4endl;
	}
	G4cout<<"============="<<G4endl;

	// Filling Histograms
	for(int i = 0; i < NUMLAYERS; i++){
		hProfileEGap->Fill(i,(thisRunTotEGap[i]/numEvents)/keV);
		hProfileEAbs->Fill(i,(thisRunTotEAbs[i]/numEvents)/keV);
		hProfileNumSec->Fill(i,thisRunTotNumSecondaries[i]/numEvents);
	}

	// Writing Histograms
	outfile->Write();
	outfile->Close();
	delete outfile;
}
