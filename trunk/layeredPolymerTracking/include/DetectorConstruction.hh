#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "DetectorMessenger.hh"

class G4Box;
class G4VPhysicalVolume;
class G4UniformMagField;

/// Detector construction class to define materials and geometry.
/// The calorimeter is a box made of a given number of layers. A layer consists
/// of an absorber plate and of a detection gap. The layer is replicated.
///
/// Four parameters define the geometry of the calorimeter :
///
/// - the thickness of an absorber plate,
/// - the thickness of a gap,
/// - the number of layers,
/// - the transverse size of the calorimeter (the input face is a square).
///
/// In DefineVolumes(), sensitive detectors of G4MultiFunctionalDetector type
/// with primitive scorers are created and associated with the Absorber 
/// and Gap volumes.


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

     
  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    // data members
    DetectorMessenger  fMessenger; // messenger 

    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
};

#endif

