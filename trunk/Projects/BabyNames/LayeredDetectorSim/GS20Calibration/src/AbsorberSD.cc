#include "AbsorberSD.hh"
#include "AbsorberHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"
#include "G4VProcess.hh"

AbsorberSD::AbsorberSD(const G4String& name,const G4String& HCname) :
  G4VSensitiveDetector(name),hitCollection(NULL) 
{

    collectionName.insert(HCname);
}


AbsorberSD::~AbsorberSD(){ }


void AbsorberSD::Initialize(G4HCofThisEvent* HCE){

  // Create Hits Collection
  hitCollection = new AbsHitsCollection(SensitiveDetectorName,collectionName[0]); 
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, hitCollection );
}

/**
 * ProcessHits
 *
 * Adds a hit to the sensitive detector, depending on the step
 */
G4bool AbsorberSD::ProcessHits(G4Step* aStep,G4TouchableHistory*){

  AbsorberHit* newHit = new AbsorberHit();

  newHit->SetTrackID    (aStep->GetTrack()->GetTrackID());
  newHit->SetParentID   (aStep->GetTrack()->GetParentID());
  newHit->SetEdep		    (aStep->GetTotalEnergyDeposit());
  newHit->SetPosition	  (aStep->GetPreStepPoint()->GetPosition());
  newHit->SetMomentum	  (aStep->GetPreStepPoint()->GetMomentum());
  newHit->SetKineticEnergy (aStep->GetPreStepPoint()->GetKineticEnergy());
  newHit->SetParticle   (aStep->GetTrack()->GetDefinition());
  newHit->SetVolume		  (aStep->GetTrack()->GetVolume());
  hitCollection->insert( newHit );

  return true;
}


void AbsorberSD::EndOfEvent(G4HCofThisEvent*){
  if (verboseLevel > 1){
    G4int nOfHits = hitCollection->entries();
    G4cout<<"\n-----> Hits Collection: in this event they are "<<
      nOfHits<<" hits:"<<G4endl;
    for (G4int i=0; i<nOfHits; i++) (*hitCollection)[i]->Print();
  }
}

