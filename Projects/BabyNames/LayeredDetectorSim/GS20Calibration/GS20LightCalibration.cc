/// \file optical//.cc
/// \brief Main program of the optical/ example
//
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4String.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "RunAction.hh"
#include "SteppingVerbose.hh"

#include "RecorderBase.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc, char** argv)
{
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);

  G4RunManager* runManager = new G4RunManager;

  runManager->SetUserInitialization(new DetectorConstruction);
  runManager->SetUserInitialization(new PhysicsList);

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  RecorderBase* recorder = NULL;//No recording is done in this example

  runManager->SetUserAction(new PrimaryGeneratorAction);
  runManager->SetUserAction(new StackingAction);

  runManager->SetUserAction(new RunAction(recorder));
  runManager->SetUserAction(new EventAction(recorder));
  runManager->SetUserAction(new TrackingAction(recorder));
  runManager->SetUserAction(new SteppingAction(recorder));

  // runManager->Initialize();
 
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if(argc==1){
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif
  }
  else{
    G4String command = "/control/execute ";
    G4String filename = argv[1];
    UImanager->ApplyCommand(command+filename);
  }

  if(recorder)delete recorder;

#ifdef G4VIS_USE
  delete visManager;
#endif

  // job termination
  delete runManager;
  return 0;
}
