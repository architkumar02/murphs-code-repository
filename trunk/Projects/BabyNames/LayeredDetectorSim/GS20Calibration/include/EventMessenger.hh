/// \file optical//include/EventMessenger.hh
/// \brief Definition of the EventMessenger class
//
//
#ifndef EventMessenger_h
#define EventMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class EventAction;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

class EventMessenger: public G4UImessenger
{
  public:

    EventMessenger(EventAction*);
    virtual ~EventMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:

    EventAction*        fEvent;
    G4UIcmdWithAnInteger*  fSaveThresholdCmd;
    G4UIcmdWithAnInteger*  fVerboseCmd;
    G4UIcmdWithAnInteger*  fPmtThresholdCmd;
    G4UIcmdWithABool*      fForceDrawPhotonsCmd;
    G4UIcmdWithABool*      fForceDrawNoPhotonsCmd;
};

#endif
