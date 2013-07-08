/// \file optical//include/SteppingAction.hh
/// \brief Definition of the SteppingAction class
//
#ifndef SteppingAction_H
#define SteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

#include "G4OpBoundaryProcess.hh"

class RecorderBase;
class EventAction;
class TrackingAction;
class SteppingMessenger;

class SteppingAction : public G4UserSteppingAction
{
  public:

    SteppingAction(RecorderBase*);
    virtual ~SteppingAction();
    virtual void UserSteppingAction(const G4Step*);

    void SetOneStepPrimaries(G4bool b){fOneStepPrimaries=b;}
    G4bool GetOneStepPrimaries(){return fOneStepPrimaries;}
 
  private:

    RecorderBase* fRecorder;
    G4bool fOneStepPrimaries;
    SteppingMessenger* fSteppingMessenger;

    G4OpBoundaryProcessStatus fExpectedNextStatus;
};

#endif
