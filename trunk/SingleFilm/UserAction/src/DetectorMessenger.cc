#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{ 
  fB4Directory = new G4UIdirectory("/B4/");
  fB4Directory->SetGuidance("UI commands of this example");
  
  fDetDirectory = new G4UIdirectory("/B4/det/");
  fDetDirectory->SetGuidance("Detector construction control");

}


DetectorMessenger::~DetectorMessenger()
{
  delete fDetDirectory;
  delete fB4Directory;  
}


void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
}

