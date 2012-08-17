#include "CaloHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
G4Allocator<CaloHit> HitAllocator;

CaloHit::CaloHit(const G4int layer): G4VHit(), 
    layerNumber(layer){
    // Initilization preformed in intilization list
}

/**
 * Destroctor of CaloHit
 */
CaloHit::~CaloHit() {}


void CaloHit::Print(){
	G4cout << "  trackID: " << trackID <<" particle: "
        <<particle->GetParticleName()<<" rank: "<<particleRank
		<<"\nvolume: "<<volume->GetName()<<" layer number: "<<layerNumber
        <<" position[mm]: " << pos
		<<"\nenergy deposit[MeV]: " << edep<<" Momentum: "<<momentum
		<<" step length [mm]: "<< stepLength/mm<<G4endl;
}

