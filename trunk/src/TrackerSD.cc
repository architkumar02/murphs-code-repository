#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "RootIO.hh"


TrackerSD::TrackerSD(G4String name) :G4VSensitiveDetector(name) {
  G4String HCname;
  collectionName.insert(HCname="trackerCollection");
}


TrackerSD::~TrackerSD(){ 
  RootIO::GetInstance()->Close();
}


void TrackerSD::Initialize(G4HCofThisEvent* HCE){
  trackerCollection = new HitsCollection(SensitiveDetectorName,collectionName[0]); 
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, trackerCollection ); 
}


G4bool TrackerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*){
  G4double edep = aStep->GetTotalEnergyDeposit();

  if(edep==0.) return false;

  TrackerHit* newHit = new TrackerHit();
  newHit->SetTrackID	(aStep->GetTrack()->GetTrackID());
  newHit->SetEdep		(edep);
  newHit->SetStepLength	(aStep->GetStepLength());
  newHit->SetMomentum	(aStep->GetTrack()->GetMomentum());
  newHit->SetParticle	(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  newHit->SetPosition	(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetVolume		(aStep->GetTrack()->GetVolume()->GetName());
  trackerCollection->insert( newHit );
  
  //newHit->Print();
  //newHit->Draw();

  return true;
}


void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  // storing the hits in ROOT file
  G4int NbHits = trackerCollection->entries();
  std::vector<TrackerHit*> hitsVector;

  { 
    G4cout << "\n-------->Storing hits in the ROOT file: in this event there are " << NbHits 
           << " hits in the tracker chambers: " << G4endl;
    for (G4int i=0;i<NbHits;i++) (*trackerCollection)[i]->Print();
  } 

  
  for (G4int i=0;i<NbHits;i++) 
    hitsVector.push_back((*trackerCollection)[i]);
  
  RootIO::GetInstance()->Write(&hitsVector);

}

