#ifndef AbsorberHit_h
#define AbsorberHit_h 1

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
 *  - momentum and kinetic energy
 *  - geometric information
 */
class AbsorberHit : public G4VHit {
	public:
		AbsorberHit();
		~AbsorberHit();

		inline void* operator new(size_t);
		inline void operator delete(void*);

		void Print();

	private:
		G4ThreeVector pos;			            /* Position of the hit                    */
		G4ThreeVector momentum;		          /* Momentrum of the ste                   */
		G4double kEnergy;                   /* Kinetic Energy of the particle         */
		G4double edep;				              /* Energy Deposited at the Hit            */
		G4int trackID;				              /* Track ID                               */
		G4int parentID;                     /* Parent ID                              */
    G4double time;                      /* Global time (time since current event) */
    G4ParticleDefinition* particle;     /* Particle Definition                    */
		G4int particleRank;                 /* Primary, Secondary, etc                */
		G4VPhysicalVolume* volume;			    /* Physical Volume                        */

	public:
		void SetTrackID(G4int track)				  {trackID = track;};
		void SetParentID(G4int id)				    {parentID = id;};
		void SetPosition(G4ThreeVector p)			{pos = p;};
		void SetMomentum(G4ThreeVector p)			{momentum = p;};
		void SetKineticEnergy(G4double E)     {kEnergy = E;};
		void SetEdep(G4double de)					    {edep = de;};
    void SetTime(G4double t)              {time = t;};
		void SetParticle(G4ParticleDefinition* pdef)
		{particle = pdef;};
		void SetParticleRank(G4int rank)      {particleRank = rank;};
		void SetVolume(G4VPhysicalVolume* v)	{volume = v;};

		G4int GetTrackID()					        {return trackID;};
    G4int GetParentID()                 {return parentID;};
		G4ThreeVector GetPosition()			    {return pos;};
		G4ThreeVector GetMomentum()			    {return momentum;};
		G4double GetEdep()					        {return edep;};
    G4double GetTime()                  {return time;};
		G4double GetKineticEnergy()         {return kEnergy;};
		G4ParticleDefinition* GetParticle() {return particle;};
		G4int GetParticleRank()             {return particleRank;};
		G4VPhysicalVolume* GetVolume()      {return volume;};
};

typedef G4THitsCollection<AbsorberHit> AbsHitsCollection;
extern G4Allocator<AbsorberHit> AbsHitAllocator;

/**
 * Memory Allocator
 */
inline void* AbsorberHit::operator new(size_t){
	void *aHit;
	aHit = (void *) AbsHitAllocator.MallocSingle();
	return aHit;
}
/**
 * Memory Deallocator
 */
inline void AbsorberHit::operator delete(void *aHit){
	AbsHitAllocator.FreeSingle((AbsorberHit*) aHit);
}
#endif

