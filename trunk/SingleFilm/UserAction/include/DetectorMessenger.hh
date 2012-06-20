#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

/// Messenger class that defines commands for DetectorConstruction.
///
/// It implements commands:
/// - /B4/det/setMagField value unit

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* detectorConstruction);
    virtual ~DetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConstruction*  fDetectorConstruction;
    
    G4UIdirectory*             fB4Directory;
    G4UIdirectory*             fDetDirectory;
};

#endif

