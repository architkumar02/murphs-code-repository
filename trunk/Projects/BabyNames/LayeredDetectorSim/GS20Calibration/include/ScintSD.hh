/// \file optical//include/ScintSD.hh
/// \brief Definition of the ScintSD class
//
//
#ifndef ScintSD_h
#define ScintSD_h 1

#include "ScintHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class ScintSD : public G4VSensitiveDetector
{
  public:

    ScintSD(G4String name);
    virtual ~ScintSD();

    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();
 
  private:

    ScintHitsCollection* fScintCollection;
 
};

#endif
