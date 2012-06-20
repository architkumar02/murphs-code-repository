#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "HadronPhysicsQGSC_BERT.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4IonPhysics.hh"
#include "OpticalPhysics.hh"
#include "HadronicPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"


PhysicsList::PhysicsList() : G4VModularPhysicsList(){
	// Registering the physics
	RegisterPhysics(new G4DecayPhysics());			
	RegisterPhysics(new G4IonPhysics());			// Duetrons, tritons, alphas
//	RegisterPhysics(new OpticalPhysics("Optical"));
	RegisterPhysics(new G4OpticalPhysics());
//	RegisterPhysics(new HadronicPhysics("Neutron"));
	RegisterPhysics(new HadronPhysicsQGSP_BERT_HP());

	RegisterPhysics(new G4EmStandardPhysics());
	
//	RegisterPhysics(new G4QStoppingPhysics());		// TODO: Do I really want a CHiral Phase Invariant Space Model here?
//	RegisterPhysics(new HadronPhysicsQGSC_BERT());
	
			
	

	//RegisterPhysics(new G4NeutronHPInelastic());

	defaultCutValue = 1*um;		// Setting Default Cut Value
	SetVerboseLevel(1);
}

PhysicsList::~PhysicsList(){
	// Empty
}

void PhysicsList::SetCuts(){
	// Changed the default range cut to 0.1 um
	SetDefaultCutValue(0.1*um);
	SetCutsWithDefault();
}
