#ifndef TrackerHit_h
#define TrackerHit_h 1

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ParticleDefinition.hh"


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
class TrackerHit : public G4VHit {
public:
	TrackerHit();
	~TrackerHit();
	TrackerHit(const TrackerHit &right);

	const TrackerHit& operator=(const TrackerHit &right);			// Allows for hits to be added
	int operator==(const TrackerHit &right) const;			// Comparison between hits

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
	G4String particle;	/* Particle */
	G4String volume;			/* Name of Volume */

public:
	void SetTrackID(G4int track)				{trackID = track;};
	void SetEdep(G4double de)					{edep = de;};
	void SetPosition(G4ThreeVector p)			{pos = p;};
	void SetStepLength(G4double dl)				{stepLength = dl;};
	void SetMomentum(G4ThreeVector p)			{momentum = p;};
	void SetParticle(G4String p)	{particle = p;};
	void SetVolume(G4String v)					{volume = v;};

	G4int GetTrackID()					{return trackID;};
	G4ThreeVector GetPosition()			{return pos;};
	G4ThreeVector GetMomentum()			{return momentum;};
	G4double GetEdep()					{return edep;};
	G4double GetStepLength()			{return stepLength;};
	G4String GetParticle()				{return particle;};
	G4String GetVolume()				{return volume;};
};


typedef G4THitsCollection<TrackerHit> HitsCollection;
extern G4Allocator<TrackerHit> HitAllocator;

inline void* TrackerHit::operator new(size_t){
  void *aHit;
  aHit = (void *) HitAllocator.MallocSingle();
  return aHit;
}


inline void TrackerHit::operator delete(void *aHit){
  HitAllocator.FreeSingle((TrackerHit*) aHit);
}
#endif