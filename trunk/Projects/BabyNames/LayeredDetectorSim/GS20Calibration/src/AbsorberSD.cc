#include "AbsorberSD.hh"
#include "AbsorberHit.hh"
#include "PhotonHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

AbsorberSD::AbsorberSD(const G4String& name,const G4String& HCname) :
  G4VSensitiveDetector(name),hitCollection(NULL) 
{

    collectionName.insert(HCname);
}


AbsorberSD::~AbsorberSD(){ }


void AbsorberSD::Initialize(G4HCofThisEvent* HCE){

  // Create Hits Collection
  G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hitCollection = new AbsHitsCollection(SensitiveDetectorName,collectionName[0]); 
  photonHitCollection = new PhotonHitsCollection(SensitiveDetectorName,collectionName[0]); 
  HCE->AddHitsCollection( HCID, hitCollection );
  HCE->AddHitsCollection( HCID, photonHitCollection );
}

/**
 * ProcessHits
 *
 * Adds a hit to the sensitive detector, depending on the step
 */
G4bool AbsorberSD::ProcessHits(G4Step* aStep,G4TouchableHistory* hist){
    
    // Making sure step exits
    if (aStep == NULL) return false;

    // Making sure it is an optical photon
    G4Track* theTrack = aStep->GetTrack();
    if(theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
      return ProcessOpticalPhotonHit(aStep,hist);
    else
      return ProcessAbsorberHit(aStep,hist);
}
/**
 * Creates a hit for an absorber particle
 */
G4bool AbsorberSD::ProcessAbsorberHit(G4Step* aStep,G4TouchableHistory*){
  AbsorberHit* newHit = new AbsorberHit();

  newHit->SetEdep		    (aStep->GetTotalEnergyDeposit());
  newHit->SetPosition	  (aStep->GetPreStepPoint()->GetPosition());
  newHit->SetMomentum	  (aStep->GetPreStepPoint()->GetMomentum());
  newHit->SetKineticEnergy (aStep->GetPreStepPoint()->GetKineticEnergy());
  newHit->SetParticle   (aStep->GetTrack()->GetDefinition());
  hitCollection->insert( newHit );

  return true;
}
/**
 * Creates a hit for optical photons
 *
 * @returns true if the hit was made, false otherwise
 */
G4bool AbsorberSD::ProcessOpticalPhotonHit(G4Step* aStep,G4TouchableHistory*){
    // Setting hit informaiton
    PhotonHit* newHit = new PhotonHit();
    newHit->SetPosition	  (aStep->GetPreStepPoint()->GetPosition());
    newHit->SetKineticEnergy (aStep->GetPreStepPoint()->GetKineticEnergy());
    newHit->SetArrivalTime(aStep->GetTrack()->GetGlobalTime());
    photonHitCollection->insert( newHit );

    return true;
}
/**
 * EndOfEvent - called for the hit at the end of the event
 *
 */
void AbsorberSD::EndOfEvent(G4HCofThisEvent*){
  if (verboseLevel > 1){
    G4int nOfAbsHits = hitCollection->entries();
    G4int nOfPhotonHits = photonHitCollection->entries();
    G4cout<<"\n-----> Hits Collection:"
          <<"\n\tNumber of absorber hits: "<<nOfAbsHits
          <<"\n\tNumber of photon hits: "<<nOfPhotonHits
          <<G4endl;
  }
}

