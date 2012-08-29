
#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

SteppingAction::SteppingAction() : G4UserSteppingAction(){ 
    // Nothing to be done
}


SteppingAction::~SteppingAction() { }


void SteppingAction::UserSteppingAction(const G4Step* step){
    // Want to kill certian processes    
    if( step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() 
        == "e-_G4DNAIonisation"){
        step->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
    }
}
