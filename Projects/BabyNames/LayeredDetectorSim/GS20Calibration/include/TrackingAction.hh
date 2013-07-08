/// \file optical//include/TrackingAction.hh
/// \brief Definition of the TrackingAction class
//
//
#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class RecorderBase;

class TrackingAction : public G4UserTrackingAction {

  public:

    TrackingAction(RecorderBase*);
    virtual ~TrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:

    RecorderBase* fRecorder;

};

#endif
