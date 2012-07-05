#include "EventAction.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

EventAction::EventAction() : G4UserEventAction(){
    // Nothing to be Done Here
}



void EventAction::BeginOfEventAction(const G4Event* event){
    G4cout<<"Starting Event: "<<event->GetEventID()<<G4endl;
    
    Analysis::GetInstance()->PrepareNewEvent(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event){  

    Analysis::GetInstance()->EndOfEvent(event);
}  
