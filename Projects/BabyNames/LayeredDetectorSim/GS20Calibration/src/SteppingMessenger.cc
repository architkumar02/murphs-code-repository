/// \file optical//src/SteppingMessenger.cc
/// \brief Implementation of the SteppingMessenger class
//
//
#include "SteppingMessenger.hh"
#include "SteppingAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingMessenger::SteppingMessenger(SteppingAction* step)
 : fStepping(step)
{
  fOneStepPrimariesCmd = new G4UIcmdWithABool("//oneStepPrimaries",this);
  fOneStepPrimariesCmd->SetGuidance("Only allows primaries to go one step in the scintillator volume before being killed.");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingMessenger::~SteppingMessenger(){
  delete fOneStepPrimariesCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void 
SteppingMessenger::SetNewValue(G4UIcommand* command,G4String newValue){
  if( command == fOneStepPrimariesCmd ){
    fStepping->SetOneStepPrimaries(fOneStepPrimariesCmd
                                  ->GetNewBoolValue(newValue));
  }
}
