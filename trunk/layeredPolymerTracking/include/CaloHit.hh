#ifndef CaloHit_h
#define CaloHit_h 1

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ParticleDefinition.hh"
#include "G4VPhysicalVolume.hh"

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

		void Print();

	private:
		G4double edep;				        /* Energy Deposited at the Hit */
		G4ThreeVector pos;			        /* Position of the hit */
		G4double stepLength;		        /* Step Length */
		G4ThreeVector momentum;		        /* Momentrum of the step */
		G4double energy;                    /* Energy of the hit */
		G4int trackID;				        /* Track ID */
		G4ParticleDefinition* particle;     /* Particle Definition */
		G4int particleRank;                 /* Primary, Secondary, etc particle */
		G4VPhysicalVolume* volume;			/* Physical Volume */
		G4int layerNumber;                  /* Copy Number of Layer */

	public:
		void SetTrackID(G4int track)				{trackID = track;};
		void SetEdep(G4double de)					{edep = de;};
		void SetPosition(G4ThreeVector p)			{pos = p;};
		void SetStepLength(G4double dl)				{stepLength = dl;};
		void SetMomentum(G4ThreeVector p)			{momentum = p;};
		void SetEnergy(G4double E)                  {energy = E;};
		void SetParticle(G4ParticleDefinition* pdef)
		{particle = pdef;};
		void SetParticleRank(G4int rank)            {particleRank = rank;};
		void SetVolume(G4VPhysicalVolume* v)	    {volume = v;};
		void SetLayerNumber(G4int layer)            {layerNumber = layer;};

		G4int GetTrackID()					{return trackID;};
		G4ThreeVector GetPosition()			{return pos;};
		G4ThreeVector GetMomentum()			{return momentum;};
		G4double GetEnergy()                {return energy;};
		G4double GetEdep()					{return edep;};
		G4double GetStepLength()			{return stepLength;};
		G4ParticleDefinition* GetParticle() {return particle;};
		G4int GetParticleRank()             {return particleRank;};
		G4VPhysicalVolume* GetVolume()      {return volume;};
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

