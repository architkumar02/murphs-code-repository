/// \file optical//include/PMTHit.hh
/// \brief Definition of the PMTHit class
//
//
#ifndef PMTHit_h
#define PMTHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class G4VTouchable;

class PMTHit : public G4VHit
{
  public:
 
    PMTHit();
    virtual ~PMTHit();
    PMTHit(const PMTHit &right);

    const PMTHit& operator=(const PMTHit &right);
    G4int operator==(const PMTHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetDrawit(G4bool b){fDrawit=b;}
    inline G4bool GetDrawit(){return fDrawit;}

    inline void IncPhotonCount(){fPhotons++;}
    inline G4int GetPhotonCount(){return fPhotons;}

    inline void SetPMTNumber(G4int n) { fPmtNumber = n; }
    inline G4int GetPMTNumber() { return fPmtNumber; }

    inline void SetPMTPhysVol(G4VPhysicalVolume* physVol){this->fPhysVol=physVol;}
    inline G4VPhysicalVolume* GetPMTPhysVol(){return fPhysVol;}

    inline void SetPMTPos(G4double x,G4double y,G4double z){
      fPos=G4ThreeVector(x,y,z);
    }
 
    inline G4ThreeVector GetPMTPos(){return fPos;}

  private:

    G4int fPmtNumber;
    G4int fPhotons;
    G4ThreeVector fPos;
    G4VPhysicalVolume* fPhysVol;
    G4bool fDrawit;

};

typedef G4THitsCollection<PMTHit> PMTHitsCollection;

extern G4Allocator<PMTHit> PMTHitAllocator;

inline void* PMTHit::operator new(size_t){
  void *aHit;
  aHit = (void *) PMTHitAllocator.MallocSingle();
  return aHit;
}

inline void PMTHit::operator delete(void *aHit){
  PMTHitAllocator.FreeSingle((PMTHit*) aHit);
}

#endif
