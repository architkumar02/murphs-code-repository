#include "TrackingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Track.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(PrimaryGeneratorAction* prim, RunAction* run, 
                               HistoManager* histo)
:primary(prim), runAction(run), histoManager(histo)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track*)
{
  //  G4cout << "ID= " << aTrack->GetTrackID() << "  e(MeV)= " 
  //         << aTrack->GetDynamicParticle()->GetKineticEnergy()/MeV << "  "
  //         << aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName()
  //         << G4endl;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  //increase nb of processed tracks 
  //count nb of steps of this track
  G4int   nbSteps = aTrack->GetCurrentStepNumber();
  G4double Trleng = aTrack->GetTrackLength();
    
  if (aTrack->GetDefinition()->GetPDGCharge() == 0.) {
    runAction->CountTraks0(1); 
    runAction->CountSteps0(nbSteps);
  
  } else {
    runAction->CountTraks1(1); 
    runAction->CountSteps1(nbSteps);
  }
  
  //true and projected ranges for primary particle
  if (aTrack->GetTrackID() == 1) {
    runAction->AddTrueRange(Trleng);
    G4ThreeVector vertex = primary->GetParticleGun()->GetParticlePosition();    
    G4ThreeVector position = aTrack->GetPosition() - vertex;      
    runAction->AddProjRange(position.x());
    runAction->AddTransvDev(position.y());
    runAction->AddTransvDev(position.z());
    
    histoManager->FillHisto(1,Trleng);
    histoManager->FillHisto(2,(float)nbSteps);    
  }        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

