#include "G4RunManager.hh"
#include "G4UImanager.hh"

// my application
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "PhysicsList.hh"

// MPI session
#include "G4MPImanager.hh"
#include "G4MPIsession.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

int main(int argc,char** argv)
{
  // random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // --------------------------------------------------------------------
  // MPI session
  // --------------------------------------------------------------------
  // At first, G4MPImanager/G4MPIsession should be created.
  G4MPImanager* g4MPI= new G4MPImanager(argc,argv);

  // MPI session (G4MPIsession) instead of G4UIterminal
  // Terminal availability depends on your MPI implementation.
  G4MPIsession* session= g4MPI-> GetMPIsession();

  // LAM/MPI users can use G4tcsh.
  G4String prompt= "[40;01;33m";
  prompt+= "G4MPI";
  prompt+= "[40;31m(%s)[40;36m[%/][00;30m:";
  session-> SetPrompt(prompt);

  // --------------------------------------------------------------------
  // user application setting
  // --------------------------------------------------------------------
  G4RunManager * runManager = new G4RunManager;
  
  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new PhysicsList);
  runManager->SetUserAction(new PrimaryGeneratorAction());
  runManager->SetUserAction(new RunAction());
  runManager->SetUserAction(new EventAction());
  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisExecutive* visManager= new G4VisExecutive;
  visManager-> Initialize();
  G4cout << G4endl;
#endif

  // --------------------------------------------------------------------
  // ready for go
  // MPIsession treats both interactive and batch modes.
  // Just start your session as below.
  // --------------------------------------------------------------------
  session-> SessionStart();


  // termination
#ifdef G4VIS_USE
  delete visManager;
#endif

  delete g4MPI;

  delete runManager;

  return 0;
}
