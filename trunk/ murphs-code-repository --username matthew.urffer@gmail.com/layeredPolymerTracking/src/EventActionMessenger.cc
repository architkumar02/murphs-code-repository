#include "EventActionMessenger.hh"
#include "EventAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"


EventActionMessenger::EventActionMessenger(EventAction* eventAction)
 : G4UImessenger(),
   fEventAction(eventAction),
   fDirectory(0),
   fSetPrintModuloCmd(0)
{
  fDirectory = new G4UIdirectory("/B4/event/");
  fDirectory->SetGuidance("event control");
   
  fSetPrintModuloCmd 
    = new G4UIcmdWithAnInteger("/B4/event/setPrintModulo",this);
  fSetPrintModuloCmd->SetGuidance("Print events modulo n");
  fSetPrintModuloCmd->SetParameterName("EventNb",false);
  fSetPrintModuloCmd->SetRange("EventNb>0");
}


EventActionMessenger::~EventActionMessenger()
{
  delete fSetPrintModuloCmd;
  delete fDirectory;   
}


void EventActionMessenger::SetNewValue(
                                        G4UIcommand* command,G4String newValue)
{ 
  if ( command == fSetPrintModuloCmd ) {
    fEventAction->SetPrintModulo(fSetPrintModuloCmd->GetNewIntValue(newValue));
  }
}

