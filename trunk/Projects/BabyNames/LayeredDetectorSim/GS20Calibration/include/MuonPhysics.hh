/// \file optical//include/MuonPhysics.hh
/// \brief Definition of the MuonPhysics class
//
//
#ifndef MuonPhysics_h
#define MuonPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuIonisation.hh"
#include "G4hIonisation.hh"

#include "G4MuonMinusCaptureAtRest.hh"

class MuonPhysics : public G4VPhysicsConstructor
{
  public:

    MuonPhysics(const G4String& name="muon");
    virtual ~MuonPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();

  protected:

   // Muon physics
   G4MuIonisation*         fMuPlusIonisation;
   G4MuMultipleScattering* fMuPlusMultipleScattering;
   G4MuBremsstrahlung*     fMuPlusBremsstrahlung;
   G4MuPairProduction*     fMuPlusPairProduction;

   G4MuIonisation*         fMuMinusIonisation;
   G4MuMultipleScattering* fMuMinusMultipleScattering;
   G4MuBremsstrahlung*     fMuMinusBremsstrahlung;
   G4MuPairProduction*     fMuMinusPairProduction;

   G4MuonMinusCaptureAtRest* fMuMinusCaptureAtRest;

};

#endif
