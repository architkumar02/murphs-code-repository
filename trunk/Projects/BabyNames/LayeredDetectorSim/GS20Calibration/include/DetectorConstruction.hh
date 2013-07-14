#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VSolid.hh"
#include "CaloSensitiveDetector.hh"

class G4Box;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;
class Material;

class DetectorConstruction : public G4VUserDetectorConstruction{

	public:
		DetectorConstruction();
		virtual ~DetectorConstruction();

		virtual G4VPhysicalVolume* Construct();                     /* Construct the detector */

		G4double GetGS20Thickness()		    {return gs20Thickness;};  /* GS20 Thickness      */
		G4double GetGS20Radius()			    {return gs20Radius;};     /* GS20 Radius         */
		G4Material* GetAbsorberMaterial()		{return absMaterial;};

		void SetGS20Thickness(G4double);

		void UpdateGeometry();
		void PrintParameters();

private:

    /* METHODS */
		void DefineMaterials();                       /* Define Materials                 */
		G4VPhysicalVolume* ConstructDetector();       /* Constructs the detector          */
		void SetVisAttributes();                      /* Sets visulationation attributes  */
		void SetSensitiveDetectors();                 /* Sets up sensitive detectors      */

    /* FIELDS */
		G4LogicalVolume*    worldLV;		// World
		G4VPhysicalVolume*  worldPV;
		G4VSolid*           worldS;

    G4LogicalVolume*    gs20LV;			// GS20 Volume
		G4VPhysicalVolume*  gs20PV;			
		G4VSolid*	          gs20S;

    G4LogicalVolume*    pmtLV;      /* PMT */
    G4VPhysicalVolume*  pmtPV;
    G4VSolid*           pmtS;
    
		// Materials
		Material* materials;

		// Geometry parameters
    G4double gs20Thickness;	      	// Thickness of Absorber
		G4double refThickness;        	// Thickness of Reflector
		G4double gs20Radius;		    	  // Outer Radius of Detector
    G4double pmtRadius;             // Radius of the PMT
		G4double detectorThickness;         // Thickness of entire detectorrimeter
		G4double worldSizeXY;
		G4double worldSizeZ;

		// Sensitive Detectors
		CaloSensitiveDetector* caloSD;

		G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
		DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

