#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VSolid.hh"

class G4Box;
class G4Tubs;
class G4UnionSolid;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;
class Materials;

class AbsorberSD;
class PMTSD;

class DetectorConstruction : public G4VUserDetectorConstruction{

	public:
		DetectorConstruction();
		virtual ~DetectorConstruction();

		virtual G4VPhysicalVolume* Construct();                     /* Construct the detector         */
    G4VPhysicalVolume* ConstructVolumes();                      /* Construct the detector volumes */

		G4double GetGS20Thickness()		    {return gs20Thickness;};  /* GS20 Thickness      */
		G4double GetGS20Radius()			    {return gs20Radius;};     /* GS20 Radius         */
		G4Material* GetAbsorberMaterial()	{return absMaterial;};    /* Returns the Abosrber*/

		void SetGS20Thickness(G4double);
    void SetGS20Radius(G4double);

		void UpdateGeometry();
		void PrintParameters();

private:

    /* METHODS */
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
		Materials* materials;            /* Material Manager                   */
    G4Material* absMaterial;        /* Absorber and Scintillator Material */

		// Geometry parameters
    G4double gs20Thickness;	      	// Thickness of Absorber
		G4double refThickness;        	// Thickness of Reflector
		G4double gs20Radius;		    	  // Outer Radius of Detector
    G4double pmtRadius;             // Radius of the PMT
		G4double detectorThickness;         // Thickness of entire detectorrimeter
    G4double pmtThickness;          /* Thickness of the PMT Glass       */
    G4double greaseThickness;       /* Thickness of the optical grease */
    G4double capThickness;          /* Thickness of the cap    */
		G4double worldSizeXY;
		G4double worldSizeZ;

    G4Material* detMaterial;        /* Detector Material */
		
    // Sensitive Detectors
		AbsorberSD* absSD;              /* Absorber (GS20 Glass SD) */
    PMTSD*  pmtSD;                  /* PMT SD                   */

		G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
		DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

