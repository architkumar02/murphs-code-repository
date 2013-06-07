#include "SteppingAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"

#include "G4SteppingManager.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(RunAction* run, EventAction* event, HistoManager* histo)
:runAction(run), eventAction(event), histoManager(histo)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4double EdepStep = aStep->GetTotalEnergyDeposit();
  if (EdepStep > 0.) {  runAction->AddEdep(EdepStep);
                      eventAction->AddEdep(EdepStep);
  }
  const G4VProcess* process = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if (process) runAction->CountProcesses(process->GetProcessName());

  // step length of primary particle
  G4int ID         = aStep->GetTrack()->GetTrackID();
  G4double steplen = aStep->GetStepLength();
  if (ID == 1) histoManager->FillHisto(3,steplen);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


