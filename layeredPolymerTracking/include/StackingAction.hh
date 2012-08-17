/**
 * @file    StackingAction.hh
 *
 * @date    5 July 2012
 * @author  murffer
 *
 * @brief   Implements user StactionAction
 */
#ifndef StackingAction_h
#define StackingAction_h

#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;

/*!
 * \brief User's StackingAction class
 * This class is used tog et access to a new G4Track
 */
class StackingAction : public G4UserStackingAction {

public:
    StackingAction();
    virtual ~StackingAction();
    //! Called for each new G4Track
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* track);

private:
};
#endif
