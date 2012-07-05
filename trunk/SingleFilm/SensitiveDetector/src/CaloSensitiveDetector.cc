#include "CaloSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"

CaloSensitiveDetector::CaloSensitiveDetector(G4String name) :G4VSensitiveDetector(name) {
  
 
  G4String HCname = hitCollection;
  collectionName.insert(HCname="hitCollection");
  G4cout<<"Creating SD with name: "<<name<<G4endl;
}


CaloSensitiveDetector::~CaloSensitiveDetector(){ }


void CaloSensitiveDetector::Initialize(G4HCofThisEvent* HCE){
  hitCollection = new HitsCollection(SensitiveDetectorName,collectionName[0]); 
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, hitCollection ); 
}


G4bool CaloSensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*){
  
  // Energy Deposition.  If no energy is depsoited then we can leave
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return true;

  CaloHit* newHit = new CaloHit();
  newHit->SetTrackID	(aStep->GetTrack()->GetTrackID());
  newHit->SetEdep		(edep);
  newHit->SetStepLength	(aStep->GetStepLength());
  newHit->SetMomentum	(aStep->GetTrack()->GetMomentum());
  newHit->SetParticle	(aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
  newHit->SetPosition	(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetVolume		(aStep->GetTrack()->GetVolume()->GetName());
  hitCollection->insert( newHit );
  
  //newHit->Print();
  //newHit->Draw();

  return true;
}


void CaloSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    hitCollection->PrintAllHits();

}

