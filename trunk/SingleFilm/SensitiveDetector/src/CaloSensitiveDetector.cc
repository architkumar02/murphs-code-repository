#include "CaloSensitiveDetector.hh"
#include "CaloHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"

#include "Analysis.hh"

CaloSensitiveDetector::CaloSensitiveDetector(const G4String& name,
                                            const G4String& HCname) :
  G4VSensitiveDetector(name),hitCollection(NULL) {
  
  collectionName.insert(HCname);
}


CaloSensitiveDetector::~CaloSensitiveDetector(){ }


void CaloSensitiveDetector::Initialize(G4HCofThisEvent* HCE){
  
  // Create Hits Collection
  hitCollection = new HitsCollection(SensitiveDetectorName,collectionName[0]); 
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, hitCollection );
}


G4bool CaloSensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*){
  
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double stepLength = aStep->GetStepLength();
  // Only saving a hit if there was energy depostion
  // if ( edep == 0. && stepLength == 0.) return false;

  // Getting the copy number
  G4TouchableHistory* touchable = (G4TouchableHistory*)
        (aStep->GetPreStepPoint()->GetTouchable());
  G4int layerIndex = touchable->GetReplicaNumber(1);
 
    
  CaloHit* newHit = new CaloHit(layerIndex);
  newHit->SetTrackID	(aStep->GetTrack()->GetTrackID());
  newHit->SetEdep		(edep);
  newHit->SetStepLength	(stepLength);
  newHit->SetPosition	(aStep->GetPreStepPoint()->GetPosition());
  newHit->SetLayerNumber(layerIndex);
  newHit->SetMomentum	(aStep->GetPreStepPoint()->GetMomentum());
  newHit->SetEnergy     (aStep->GetPreStepPoint()->GetTotalEnergy());
  newHit->SetParticle   (aStep->GetTrack()->GetDefinition());
  newHit->SetVolume		(aStep->GetTrack()->GetVolume());
  
  
  Analysis::GetInstance()->AddHit(newHit);
  hitCollection->insert( newHit );
  
  return true;
}


void CaloSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    if (verboseLevel > 1){
        G4int nOfHits = hitCollection->entries();
        G4cout<<"\n-----> Hits Collection: in this event they are "<<
        nOfHits<<" hits:"<<G4endl;
        for (G4int i=0; i<nOfHits; i++) (*hitCollection)[i]->Print();
    }
}

