#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"

class PrimaryGeneratorAction;
class RunAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(PrimaryGeneratorAction*, RunAction*, HistoManager*);
   ~TrackingAction() {};
   
    void  PreUserTrackingAction(const G4Track*);
    void PostUserTrackingAction(const G4Track*);
    
  private:
    PrimaryGeneratorAction* primary;
    RunAction*              runAction;
    HistoManager*           histoManager;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
