#include "CaloHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
G4Allocator<CaloHit> HitAllocator;

CaloHit::CaloHit(const G4int slice): G4VHit(), 
    sliceNumber(slice){
    // Initilization preformed in intilization list
}

/**
 * Destroctor of CaloHit
 */
CaloHit::~CaloHit() {}

/**
 * Print
 *
 * Print's out information about the hit
 */
void CaloHit::Print(){
	G4cout << "  trackID: " << trackID <<" particle: "
        <<particle->GetParticleName()<<" rank: "<<particleRank
		<<"\nvolume: "<<volume->GetName()<<" slice number: "<<sliceNumber
        <<" position[mm]: " << pos/mm
        <<"\n Momentum "<<momentum<<" Kinetic Energy[keV]"<<kEnergy/keV
		<<"\nenergy deposit[keV]: " << edep/keV<<" step length [mm]: "
        << stepLength/mm
        <<G4endl;
}

