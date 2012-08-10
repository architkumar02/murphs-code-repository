#include "CaloHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4ParticleTable.hh"

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
   static G4ParticleTable* pTable  (G4ParticleTable::GetParticleTable());
   G4String pName = pTable->FindParticle(pdgID)->GetParticleName();
	G4cout << "  trackID: " << trackID <<" particle: "
        <<pName<<" rank: "<<particleRank
		<<"\nvolume: "<<volume<<" layer number: "<<layerNumber
        <<" position[mm]: " << pos
		<<"\nenergy deposit[MeV]: " << edep<<" Momentum: "<<momentum
		<<" step length [mm]: "<< stepLength/mm<<G4endl;
}

