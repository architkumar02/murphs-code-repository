/**
 * @file    StackingAction.cc
 *
 * @date    5 July 2012
 * @author  murffer
 *
 * @brief   Implements user StackingAction
 */
#include "StackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"

#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "Analysis.hh"

StackingAction::StackingAction(){}

StackingAction::~StackingAction(){}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* t){

    G4ClassificationOfNewTrack result(fUrgent);
    G4int parentID = t->GetParentID();
    if (parentID > 0){
        Analysis::GetInstance()->AddSecondary(1);
    }

    const G4ParticleDefinition* particleType = t->GetParticleDefinition();
    if( particleType == G4Gamma::GammaDefinition()){
        Analysis::GetInstance()->AddGammas(1);
    }
    else if (particleType == G4Electron::ElectronDefinition()  ){
        Analysis::GetInstance()->AddElectrons(1);
    }
    return result;
}
