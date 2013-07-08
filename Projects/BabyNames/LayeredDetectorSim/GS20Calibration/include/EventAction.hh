/// \file optical//include/EventAction.hh
/// \brief Definition of the EventAction class
//

#ifndef EventAction_h
#define EventAction_h 1

#include "EventMessenger.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;
class RecorderBase;

class EventAction : public G4UserEventAction
{
  public:

    EventAction(RecorderBase*);
    virtual ~EventAction();

  public:

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void SetSaveThreshold(G4int );

    void SetEventVerbose(G4int v){fVerbose=v;}

    void SetPMTThreshold(G4int t){fPMTThreshold=t;}

    void SetForceDrawPhotons(G4bool b){fForcedrawphotons=b;}
    void SetForceDrawNoPhotons(G4bool b){fForcenophotons=b;}

  private:

    RecorderBase* fRecorder;
    EventMessenger* fEventMessenger;

    G4int              fSaveThreshold;

    G4int              fScintCollID;
    G4int              fPMTCollID;

    G4int              fVerbose;

    G4int              fPMTThreshold;

    G4bool fForcedrawphotons;
    G4bool fForcenophotons;

};

#endif
