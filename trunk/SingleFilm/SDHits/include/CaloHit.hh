#ifndef CaloHit_h
#define CaloHit_h 1

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"


/**
 * @brief - Hit: a snapshot of the physcial interaction of a track in the sensitive region of a detector
 *
 * Contians:
 *  - Particle Information (type and rank (primary, secondary, tertiary ...))
 *  - Positon and time
 *  - momentum and energy
 *  - energy deposition in volume
 *  - geometric information
 */
class CaloHit : public G4VHit {
public:
	CaloHit(const G4int layer);
	~CaloHit();


	inline void* operator new(size_t);
	inline void operator delete(void*);

	void Draw();
	void Print();

private:
	G4double edep;				/* Energy Deposited at the Hit */
	G4ThreeVector pos;			/* Position of the hit */
	G4double stepLength;		/* Step Length */
	G4ThreeVector momentum;		/* Momentrum of the step */
	G4int trackID;				/* Track ID */
	G4int pdgID;	            /* Particle PDG*/
    G4int particleRank;         /* Primary, Secondary, Teritiary particle */
	G4String volume;			/* Name of Volume */
    G4int layerNumber;          /* Copy Number of Layer */

public:
	void SetTrackID(G4int track)				{trackID = track;};
	void SetEdep(G4double de)					{edep = de;};
	void SetPosition(G4ThreeVector p)			{pos = p;};
	void SetStepLength(G4double dl)				{stepLength = dl;};
	void SetMomentum(G4ThreeVector p)			{momentum = p;};
	void SetParticle(G4int p)	                {pdgID = p;};
	void SetParticleRank(G4int rank)            {particleRank = rank;};
    void SetVolume(G4String v)					{volume = v;};
    void SetLayerNumber(G4int layer)            {layerNumber = layer;};

	G4int GetTrackID()					{return trackID;};
	G4ThreeVector GetPosition()			{return pos;};
	G4ThreeVector GetMomentum()			{return momentum;};
	G4double GetEdep()					{return edep;};
	G4double GetStepLength()			{return stepLength;};
	G4int GetParticle()	                {return pdgID;};
    G4int GetParticleRank()             {return particleRank;};
	G4String GetVolume()				{return volume;};
    G4int GetLayerNumber()              {return layerNumber;};
};


typedef G4THitsCollection<CaloHit> HitsCollection;
extern G4Allocator<CaloHit> HitAllocator;

inline void* CaloHit::operator new(size_t){
  void *aHit;
  aHit = (void *) HitAllocator.MallocSingle();
  return aHit;
}


inline void CaloHit::operator delete(void *aHit){
  HitAllocator.FreeSingle((CaloHit*) aHit);
}
#endif

