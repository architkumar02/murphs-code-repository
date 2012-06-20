#ifndef OpticalPhysics_h
#define OpticalPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpMieHG.hh"


class OpticalPhysics: public G4VPhysicsConstructor

{
  public:
    OpticalPhysics(const G4String& name="Optical");
   ~OpticalPhysics();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess(); 

public:
	void SetVerbose(G4int);
    void SetNbOfPhotonsCerenkov(G4int);
private:
	G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
    G4OpMieHG*           theMieHGScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;
};

#endif



