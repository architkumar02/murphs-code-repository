#include "AbsorberHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>
G4Allocator<AbsorberHit> AbsHitAllocator;

AbsorberHit::AbsorberHit(): G4VHit(){
    // Initilization preformed in intilization list
}

/**
 * Destroctor of AbsorberHit
 */
AbsorberHit::~AbsorberHit() {}

/**
 * Print
 *
 * Print's out information about the hit
 */
void AbsorberHit::Print(){
	G4cout << "particle: "<<particle->GetParticleName()
		     <<"\nvolume: "<<volume->GetName()
         <<" position[mm]: " << pos/mm
         <<"\n Momentum "<<momentum<<" Kinetic Energy[keV]"<<kEnergy/keV
		     <<"\nenergy deposit[keV]: " << edep/keV
         <<G4endl;
}

