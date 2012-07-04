#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"


RunAction::RunAction()
	: G4UserRunAction()
{ 
}


RunAction::~RunAction()
{
}


void RunAction::BeginOfRunAction(const G4Run* run)
{ 
	G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(true);

	// Book histograms, ntuple
	//

	// Create analysis manager
	// The choice of analysis technology is done via selectin of a namespace
	// in Analysis.hh
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	// Open an output file
	//
	G4String fileName = "LayeredPolymerDetector";
	analysisManager->OpenFile(fileName);
	analysisManager->SetFirstHistoId(1);

	// Creating histograms
	//
	analysisManager->CreateH1("1","Edep in absorber", 100, 0., 5*MeV);
	analysisManager->CreateH1("2","Edep in gap", 100, 0., 5*MeV);
	analysisManager->CreateH1("3","trackL in absorber", 100, 0., 1*m);
	analysisManager->CreateH1("4","trackL in gap", 100, 0., 50*cm);

	// Creating ntuple
	//
	analysisManager->CreateNtuple("", "Edep and TrackL");
	analysisManager->CreateNtupleDColumn("Eabs");
	analysisManager->CreateNtupleDColumn("Egap");
	analysisManager->CreateNtupleDColumn("Labs");
	analysisManager->CreateNtupleDColumn("Lgap");
	analysisManager->CreateNtupleDColumn("Flux");
	analysisManager->CreateNtupleDColumn("Dose");
	analysisManager->CreateNtupleIColumn("NumSecondaries");
	analysisManager->CreateNtupleIColumn("NumElectronSecondaries");
	analysisManager->CreateNtupleIColumn("NumGammaSecondaries");
	analysisManager->CreateNtupleDColumn("EAbsoPerSecondary");
	analysisManager->CreateNtupleDColumn("EGapPerSecondary");
	analysisManager->CreateNtupleDColumn("LAbsoPerSecondary");
	analysisManager->CreateNtupleDColumn("LGapPerSecondary");
	analysisManager->FinishNtuple();
}


void RunAction::EndOfRunAction(const G4Run* aRun)
{
	G4int nofEvents = aRun->GetNumberOfEvent();
	if ( nofEvents == 0 ) return;

	// print histogram statistics
	//
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	if ( analysisManager->GetH1(1) ) {
		G4cout << "\n ----> print histograms statistic \n" << G4endl;

		G4cout 
			<< " EAbs : mean = " << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
			<< " rms = " << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") 
			<< G4endl;
		G4cout 	       
			<< " EGap : mean = " << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy") 
			<< " rms = " << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") 
			<< G4endl;
		G4cout 
			<< " LAbs : mean = " << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length") 
			<< " rms = " << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") 
			<< G4endl;
		G4cout 
			<< " LGap : mean = " << G4BestUnit(analysisManager->GetH1(4)->mean(), "Length") 
			<< " rms = " << G4BestUnit(analysisManager->GetH1(4)->rms(),  "Length") 
			<< G4endl;
	}

	// save histograms 
	//
	analysisManager->Write();
	analysisManager->CloseFile();

	// complete cleanup
	//
	delete G4AnalysisManager::Instance();  
}
