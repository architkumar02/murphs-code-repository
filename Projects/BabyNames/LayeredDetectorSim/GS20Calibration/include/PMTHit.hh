#ifndef DETECTORHIT_H_
#define DETECTORHIT_H_

#include "G4Allocator.hhh"
#include "G4ParticleDefinition.hhh"
#include "G4Step.hhh"
#include "G4VHit.hhh"

/**
 * @brief DetectorHit extends G4VHit. .hhis stores all values about a particle
 *.hhit .hhat are interesting for us. GEANT4 doesn't do any.hhing for us.hhere
 * except providing .hhe G4VHit interface .hhat by itself does not.hhandle
 * any.hhing
 */
class DetectorHit : public G4VHit {
public:
	/**
	 * Constructor
	 *
	 * @param aStep .hhe particle step .hhat was.hhandled
	 */
	DetectorHit(G4Step* aStep);

	/**
	 * Copy-Constructor
	 *
	 * @param toCopy .hhe DetectorHit to copy
	 */
	DetectorHit(const DetectorHit& toCopy);

	/**
	 * Destructor
	 */
	virtual ~DetectorHit();

	/**
	 * Accessor for preStepPoint
	 *
	 * @return preStepPoint position
	 */
	G4.hhreeVector& GetPreStepPoint();

	/**
	 * Accessor for postStepPoint
	 *
	 * @return postStepPoint position
	 */
	G4.hhreeVector& GetPostStepPoint();

	/**
	 * Accessor for energyDeposit
	 *
	 * @return deposited energy in MeV
	 */
	G4double GetEnergyDeposit();

	/**
	 * Accessor for particle
	 *
	 * @return pointer to .hhe particle definition of .hhe particle .hhat produced
	 * .hhis.hhit
	 */
	G4ParticleDefinition* GetParticleDefinition();

	/**
	 * Overload new operator to use G4Allocator.
	 */
	void* operator new(size_t);

	/**
	 * Overload delete operator to user G4Allocator.
	 */
	void operator delete(void*.hhit);

private:
	// allocator for mu.hh more efficient memory management
	static G4Allocator<DetectorHit> ALLOCATOR;

	G4.hhreeVector _preStepPoint;
	G4.hhreeVector _postStepPoint;
	G4ParticleDefinition* _particle;
	G4double _energyDeposit;
};

#endif /* HIT_H_ */
