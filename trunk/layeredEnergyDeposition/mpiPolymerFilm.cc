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
    G4MPImanager* g4MPI= new G4MPImanager(argc,argv);
    G4MPIsession* session= g4MPI-> GetMPIsession();

    // --------------------------------------------------------------------
    // user application setting
    // --------------------------------------------------------------------
    G4RunManager * runManager = new G4RunManager;
    DetectorConstruction *detector = new DetectorConstruction();  
    runManager->SetUserInitialization(detector);
    Analysis *a = new Analysis(detector);
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->SetUserAction(new RunAction(a));
    runManager->SetUserAction(new EventAction(a));
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
    delete a;
    delete detector;
    return 0;
}
