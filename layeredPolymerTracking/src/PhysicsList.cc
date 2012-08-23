#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "HadronPhysicsQGSC_BERT.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4IonPhysics.hh"
#include "HadronicPhysics.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "G4EmDNAPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4OpticalPhysics.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList(){
	// Registering the physics
	RegisterPhysics(new G4DecayPhysics());			
	RegisterPhysics(new G4IonPhysics());			// Duetrons, tritons, alphas
//	RegisterPhysics(new HadronicPhysics("Neutron"));
	RegisterPhysics(new HadronPhysicsQGSP_BERT_HP());

	//RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4EmLivermorePhysics());
    RegisterPhysics(new G4OpticalPhysics());
    RegisterPhysics(new G4EmDNAPhysics());      // Microdoisemtry	
//	RegisterPhysics(new G4QStoppingPhysics());		// TODO: Do I really want a CHiral Phase Invariant Space Model here?
//	RegisterPhysics(new HadronPhysicsQGSC_BERT());
	

	//RegisterPhysics(new G4NeutronHPInelastic());
	defaultCutValue = 0.1*nm;		// Setting Default Cut Value
}

PhysicsList::~PhysicsList(){
	// Empty
}

void PhysicsList::SetCuts(){
	// Changed the default range cut to 0.1 um
	SetDefaultCutValue(0.1*nm);
	SetCutsWithDefault();
}
