/// \file optical//include/ScintHit.hh
/// \brief Definition of the ScintHit class
//
//
#ifndef ScintHit_h
#define ScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class ScintHit : public G4VHit
{
  public:
 
    ScintHit();
    ScintHit(G4VPhysicalVolume* pVol);
    virtual ~ScintHit();
    ScintHit(const ScintHit &right);
    const ScintHit& operator=(const ScintHit &right);
    G4int operator==(const ScintHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetEdep(G4double de) { fEdep = de; }
    inline void AddEdep(G4double de) { fEdep += de; }
    inline G4double GetEdep() { return fEdep; }

    inline void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    inline G4ThreeVector GetPos() { return fPos; }

    inline const G4VPhysicalVolume * GetPhysV() { return fPhysVol; }

  private:
    G4double fEdep;
    G4ThreeVector fPos;
    const G4VPhysicalVolume* fPhysVol;

};

typedef G4THitsCollection<ScintHit> ScintHitsCollection;

extern G4Allocator<ScintHit> ScintHitAllocator;

inline void* ScintHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) ScintHitAllocator.MallocSingle();
  return aHit;
}

inline void ScintHit::operator delete(void *aHit)
{
  ScintHitAllocator.FreeSingle((ScintHit*) aHit);
}

#endif
