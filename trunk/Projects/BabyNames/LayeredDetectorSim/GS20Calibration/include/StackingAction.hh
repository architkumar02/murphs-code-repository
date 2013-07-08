/// \file optical//include/StackingAction.hh
/// \brief Definition of the StackingAction class
//
//
#ifndef StackingAction_H
#define StackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class StackingAction : public G4UserStackingAction
{
  public:

    StackingAction();
    virtual ~StackingAction();
 
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
};

#endif
