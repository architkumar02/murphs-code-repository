#include "StackingAction.hh"

#include "G4Track.hh"

StackingAction::StackingAction()
{ }


StackingAction::~StackingAction()
{ }


G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
    if(track->GetParentID() > 0){
      return fKill;
    }
    else
      return fUrgent;
}
