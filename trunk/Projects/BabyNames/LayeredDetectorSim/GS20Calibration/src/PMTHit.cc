#include "PMTHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
G4Allocator<PMTHit> PMTHitAllocator;

PMTHit::PMTHit(): G4VHit(){
    // Initilization preformed in intilization list
}

/**
 * Destroctor of PMTHit
 */
PMTHit::~PMTHit() {}

/**
 * Print
 *
 * Print's out information about the hit
 */
void PMTHit::Print(){
	G4cout << "  trackID: " << trackID <<" particle: "
         <<particle->GetParticleName()<<" rank: "<<particleRank
		     <<"\nvolume: "<<volume->GetName()
         <<" position[mm]: " << pos/mm
         <<"\n Momentum "<<momentum<<" Kinetic Energy[keV]"<<kEnergy/keV
         <<G4endl;
}

