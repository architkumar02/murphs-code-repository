#include "EventAction.hh"
#include "EventActionMessenger.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>


EventAction::EventAction()
	: G4UserEventAction(),
	fMessenger(this),
	fPrintModulo(1000)
{
}


EventAction::~EventAction()
{
}


G4THitsMap<G4double>* EventAction::GetHitsCollection(const G4String& hcName,const G4Event* event) const
{
	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
	G4THitsMap<G4double>* hitsCollection = static_cast<G4THitsMap<G4double>*>(event->GetHCofThisEvent()->GetHC(hcID));

	if ( ! hitsCollection ) {
		G4cerr << "Cannot access hitsCollection " << hcName << G4endl;
		exit(1);
	}         

	return hitsCollection;
}    

G4double EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
	G4double sumValue = 0;
	std::map<G4int, G4double*>::iterator it;
	for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++) {
		sumValue += *(it->second);
	}
	return sumValue;  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::PrintEventStatistics(
	G4double absoEdep, G4double absoTrackLength,
	G4double gapEdep, G4double gapTrackLength) const
{
	// Print event statistics
	//
	G4cout
		<< "   Absorber: total energy: " 
		<< std::setw(7) << G4BestUnit(absoEdep, "Energy")
		<< "       total track length: " 
		<< std::setw(7) << G4BestUnit(absoTrackLength, "Length")
		<< G4endl
		<< "        Gap: total energy: " 
		<< std::setw(7) << G4BestUnit(gapEdep, "Energy")
		<< "       total track length: " 
		<< std::setw(7) << G4BestUnit(gapTrackLength, "Length")
		<< G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{  

	G4int eventID = event->GetEventID();
	if ( eventID % fPrintModulo == 0 )  { 
		G4cout << "\n---> Begin of event: " << eventID << G4endl;
		//CLHEP::HepRandom::showEngineStatus();
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event){  
	G4double eMin = 5*eV;

	// Get sum value from hits collections
	G4double absoEdep  = GetSum(GetHitsCollection("Absorber/Edep", event));
	G4double gapEdep = GetSum(GetHitsCollection("Gap/Edep", event));
	G4double absoTrackLength = GetSum(GetHitsCollection("Absorber/TrackLength", event));
	G4double gapTrackLength = GetSum(GetHitsCollection("Gap/TrackLength", event));
	
	/* Only scoring events when have an energy deposition greater than eMin */
	if( absoEdep > eMin || gapEdep > eMin){

		G4double flux = GetSum(GetHitsCollection("Absorber/TotalSurfaceFlux",event));
		G4double dose = GetSum(GetHitsCollection("Absorber/TotalDose",event));

		G4int nSecondaries = GetSum(GetHitsCollection("Absorber/nSecondary",event));
		G4int nElectrons = GetSum(GetHitsCollection("Absorber/nElectron",event));
		G4int nGammas = GetSum(GetHitsCollection("Absorber/nGamma",event));

		// get analysis manager
		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

		// fill histograms
		analysisManager->FillH1(1, absoEdep);
		analysisManager->FillH1(2, gapEdep);
		analysisManager->FillH1(3, absoTrackLength);
		analysisManager->FillH1(4, gapTrackLength);

		// fill ntuple
		analysisManager->FillNtupleDColumn(0, absoEdep);
		analysisManager->FillNtupleDColumn(1, gapEdep);
		analysisManager->FillNtupleDColumn(2, absoTrackLength);
		analysisManager->FillNtupleDColumn(3, gapTrackLength);
		analysisManager->FillNtupleDColumn(4, flux);
		analysisManager->FillNtupleDColumn(5, dose);
		analysisManager->FillNtupleIColumn(6, nSecondaries);
		analysisManager->FillNtupleIColumn(7, nElectrons);
		analysisManager->FillNtupleIColumn(8, nGammas);
		analysisManager->AddNtupleRow(); 
		
		// Filling Hit NTuples
		G4THitsMap<G4double>* hitsMap = GetHitsCollection("Absorber/Edep", event);
		std::map<G4int, G4double*>::iterator it;
		for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++) {
			G4double absoEdepPerSecondary = *(it->second);
			if (absoEdepPerSecondary < 1.*eV)
				continue;
			analysisManager->FillNtupleDColumn(9, absoEdepPerSecondary);
			analysisManager->AddNtupleRow();
		}
		
		hitsMap = GetHitsCollection("Gap/Edep", event);
		for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++) {
			G4double gapEdepPerSecondary = *(it->second);
			analysisManager->FillNtupleDColumn(10, gapEdepPerSecondary);
			analysisManager->AddNtupleRow();
		}
		
		hitsMap = GetHitsCollection("Absorber/TrackLength", event);
		for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++) {
			G4double absoTrackLengthPerSecondary = *(it->second);
			analysisManager->FillNtupleDColumn(11, absoTrackLengthPerSecondary);
			analysisManager->AddNtupleRow();
		}
		
		hitsMap = GetHitsCollection("Gap/TrackLength", event);
		for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++) {
			G4double gapTrackLengthPerSecondary = *(it->second);
			analysisManager->FillNtupleDColumn(12, gapTrackLengthPerSecondary);
			analysisManager->AddNtupleRow();
		}
		
		 
	}
	//print per event (modulo n)
	//
	G4int eventID = event->GetEventID();
	if ( eventID % fPrintModulo == 0) {
		G4cout << "---> End of event: " << eventID << G4endl;     

		PrintEventStatistics(absoEdep, absoTrackLength, gapEdep, gapTrackLength);
	}
}  
