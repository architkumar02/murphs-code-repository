#ifndef CaloSensitiveDetector_h
#define CaloSensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "CaloHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class CaloSensitiveDetector : public G4VSensitiveDetector
{
  public:
      CaloSensitiveDetector(G4String);
     ~CaloSensitiveDetector();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      HitsCollection* hitCollection;

};

#endif

