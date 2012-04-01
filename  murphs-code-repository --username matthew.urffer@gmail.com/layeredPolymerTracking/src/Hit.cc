#include "Hit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<Hit> HitAllocator;

Hit::Hit() {}


Hit::~Hit() {}


Hit::Hit(const Hit& right): G4VHit(){

	edep = right.edep;
	pos = right.pos;
	stepLength = right.stepLength;
	momentum = right.momentum;
	trackID = right.trackID;
	particle = right.particle;
	volume = right.volume;
}

const Hit& Hit::operator=(const Hit& right){
	edep = right.edep;
	pos = right.pos;
	stepLength = right.stepLength;
	momentum = right.momentum;
	trackID = right.trackID;
	particle = right.particle;
	volume = right.volume;
	return *this;
}


G4int Hit::operator==(const Hit& right) const{
	return (this==&right) ? 1 : 0;
}


void Hit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	if(pVVisManager)
	{
		G4Circle circle(pos);
		circle.SetScreenSize(2.);
		circle.SetFillStyle(G4Circle::filled);
		G4Colour colour(1.,0.,0.);
		G4VisAttributes attribs(colour);
		circle.SetVisAttributes(attribs);
		pVVisManager->Draw(circle);
	}
}


void Hit::Print(){
	G4cout << "  trackID: " << trackID <<" particle: "<<particle<<
		<< "  energy deposit[MeV]: " << edep
		<< "  step length [mm]: "<< stepLength/mm
		<< "  position[mm]: " << pos <<" volume: "<<volume<<G4endl;
}

