#include "CaloHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<CaloHit> HitAllocator;

CaloHit::CaloHit(const G4int layer): 
    layerNumber(layer),
    edep(0){
    // Initilization preformed in intilization list
}


CaloHit::~CaloHit() {}



void CaloHit::Print(){
	G4cout << "  trackID: " << trackID <<" particle: "<<pid.GetParticleName()
        <<" rank: "<<particleRank
		<<"\nvolume: "<<volume<<" layer number: "<<layerNumber
        <<" position[mm]: " << pos
		<<"\nenergy deposit[MeV]: " << edep<<" Momentum: "<<momentum
		<<" step length [mm]: "<< stepLength/mm<<G4endl;
}

