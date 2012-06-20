#ifndef EventActionMessenger_h
#define EventActionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class EventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

/// Messenger class that defines commands for EventAction class.
///
/// It implements commands:
/// - /B4/event/setPrintModulo value

class EventActionMessenger: public G4UImessenger
{
public:
  EventActionMessenger(EventAction* eventAction);
  virtual ~EventActionMessenger();
    
  virtual void SetNewValue(G4UIcommand* command, G4String value);
    
private:
  EventAction*       fEventAction;
  G4UIdirectory*        fDirectory;   
  G4UIcmdWithAnInteger* fSetPrintModuloCmd;    
};
#endif
