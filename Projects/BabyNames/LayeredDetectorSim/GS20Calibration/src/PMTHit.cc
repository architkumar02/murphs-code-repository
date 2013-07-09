#include "DetectorHit.h"

#include "G4OpticalPhoton.hh"

G4Allocator<DetectorHit> DetectorHit::ALLOCATOR;

DetectorHit::DetectorHit(G4Step* aStep) {
	// construct from aStep
	_preStepPoint = aStep->GetPreStepPoint()->GetPosition();
	_postStepPoint = aStep->GetPostStepPoint()->GetPosition();
	_particle = aStep->GetTrack()->GetDefinition();
	_energyDeposit = aStep->GetTotalEnergyDeposit();

	if (_particle == G4OpticalPhoton::Definition()) {
		aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
		_energyDeposit += aStep->GetTrack()->GetMomentum().mag() * CLHEP::c_light;
	}
}

DetectorHit::DetectorHit(const DetectorHit& toCopy) :
	// copy constructor
	_preStepPoint(toCopy._preStepPoint), _postStepPoint(toCopy._postStepPoint),
			_particle(toCopy._particle), _energyDeposit(toCopy._energyDeposit) {
}

DetectorHit::~DetectorHit() {
	// this is empty
}

G4double DetectorHit::GetEnergyDeposit() {
	return _energyDeposit;
}

G4ThreeVector& DetectorHit::GetPreStepPoint() {
	return _preStepPoint;
}

G4ThreeVector& DetectorHit::GetPostStepPoint() {
	return _postStepPoint;
}

G4ParticleDefinition* DetectorHit::GetParticleDefinition() {
	return _particle;
}

void* DetectorHit::operator new(size_t) {
	return ALLOCATOR.MallocSingle();
}

void DetectorHit::operator delete(void* hit) {
	ALLOCATOR.FreeSingle((DetectorHit*) hit);
}
