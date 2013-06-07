#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class RunAction;
class EventAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(RunAction*, EventAction*, HistoManager*);
   ~SteppingAction() {};

    void UserSteppingAction(const G4Step*);
    
  private:
    RunAction*    runAction;
    EventAction*  eventAction;
    HistoManager* histoManager;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
