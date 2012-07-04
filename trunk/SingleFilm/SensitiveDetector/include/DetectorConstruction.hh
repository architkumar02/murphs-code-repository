#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "DetectorMessenger.hh"

class G4Box;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;


class DetectorConstruction : public G4VUserDetectorConstruction{

  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

     
  private:
    //! Define needed materials
    void DefineMaterials();
    //! Initialzie geometry parametners
    void ComputeParameters();
    //! Creates the Calorimeter (detector)
    G4VPhysicalVolume* ConstructCalorimeter();
    //! Sets the visualtiation attributes
    void SetVisAttributes();
    //! Sets the Sensitve Detectors
    void SetSensitiveDetectors();

    // Geometry Names
    G4LogicalVolume* worldLV;
    G4VPhysicalVolume* caloPV;
    G4VPhysicalVolume* layerPV;
    G4VPhysicalVolume* gapPV;
    G4VPhysicalVolume* absorberPV;
	
    // Materials
    G4Material* defaultMaterial;    // Vacumun
	G4Material* absorberMaterial;   // Detector material
	G4Material* gapMaterial;        // Gap material
	
    // Geometry parameters
    G4int nofLayers;                // Number of Layers
	G4double absoThickness;	        // Thickness of Absorber
	G4double gapThickness;          // Thickness of Gap 
	G4double outerRadius;		    // Outer Radius of Detector
	G4double innerRadius;			// Inner radious of  Detector
	G4double startAngle;
	G4double spanAngle;
	
	G4double layerThicknes;         // Thickness of a single layer
	G4double caloThickness;         // Thickness of entire calorimeter
    G4double worldSizeXY;
    G4double worldSizeZ;

    // data members
    DetectorMessenger  fMessenger; // messenger 
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
};

#endif

