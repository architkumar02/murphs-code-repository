#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4SDManager.hh"
#include "AbsorberSD.hh"
#include "PMTSD.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"

#include "Materials.hh"

/**
 * DetectorConstruction
 *
 * Setting the default detector construciton.
 */
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
    fCheckOverlaps(true){

        
        // Create commands for interactive defiantions of the calorimeter
        detectorMessenger = new DetectorMessenger(this);
}

/**
 * Deconstructor
 */
DetectorConstruction::~DetectorConstruction(){
  // Deleting the messenger and materials if they exist
  if (detectorMessenger) delete detectorMessenger;
  if (materials)         delete materials;
}

/**
 * Construct
 *
 * Creating the detector's world volume
 */
G4VPhysicalVolume* DetectorConstruction::Construct(){

    // Creating Detector Materials
    materials = Materials::GetInstance();
    
    // Return Physical World
    G4VPhysicalVolume* world = Construct();

    // Set Visulation Attributes
    SetVisAttributes();

    // Assign Sensitve Detectors
    SetSensitiveDetectors();
    return world;
}

/**
 * PrintCaloParameters
 *
 * Prints the parameters of the geometry
 */
void DetectorConstruction::PrintParameters(){

    // print parameters
    G4cout << "\n------------ Detector Parameters ---------------------"
        <<"\n--> The detector material is a disc of: \n\t "
        << G4BestUnit(gs20Thickness,"Length")<< " of " << absMaterial->GetName() 
        <<"\n\tLight transport properties are:"
        <<G4endl;
}

/**
 * Construct()
 *
 * Constructs the detector volume and PMT
 */
G4VPhysicalVolume* DetectorConstruction::ConstructVolumes(){

    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
        
    worldSizeZ  = 25*cm;      // Fixed World Size; 
    
    // Geometry parameters
    gs20Thickness = 2*mm;	          /* Thickness of GS20 Scintilator  */
    gs20Radius  = 2.54*cm;		      /* Radius of GS20 Sctintillator   */

    // World
    worldS = new G4Box("World",2.5*pmtRadius,2.5*pmtRadius,20*cm); 
    worldLV = new G4LogicalVolume(worldS,materials->GetMaterial("G4_Galactic"),"World");
    worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",
            0,false,0,fCheckOverlaps);

    //
    // Setting up the GS20
    //
    // The beam is shotting along the z, comming from +z

    // GS20 Detector
    gs20S = new G4Tubs("Abs",0,gs20Radius,gs20Thickness/2,0,360*deg);
    gs20LV = new G4LogicalVolume(gs20S,absMaterial,"Absorber",0);
    gs20PV = new G4PVPlacement(0,G4ThreeVector(0,0,0),gs20LV,"Absorber",worldLV,false,0,fCheckOverlaps);
    
    // Light Reflector
    G4double rInner[3] = {gs20Radius,gs20Radius,0};
    G4double rOuter[3] = {pmtRadius,gs20Radius+refThickness,gs20Radius+refThickness};
    G4double zPlane[3]  = {refThickness,gs20Thickness-refThickness,gs20Thickness+refThickness};
    G4Polycone *refS = new G4Polycone("Ref",0,2*pi,3,zPlane,rInner,rOuter);
    /*
    G4Tubs *refFlange = new G4Tubs("refFlange",gs20Radius,pmtRadius,refThickness,0,360*deg);
    G4Tubs *refSide = new G4Tubs("refSide",gs20Radius,gs20Radius+refThickness,gs20Thickness,0,360*deg);
    G4Tubs *refTop = new G4Tubs("refTop",0,gs20Radius+refThickness,refThickness,0,360*deg);
    */

    // Optical Grease
    G4double greaseThickness = 5*um;
    G4Tubs *greaseS = new G4Tubs("opticalGrease",0,gs20Thickness,greaseThickness,0,360*deg);
    G4LogicalVolume* greaseLV = new G4LogicalVolume(greaseS,materials->GetMaterial("OpticalGrease"),"PMT Glass",0);

    // PMT Glass
    G4double pmtThickness = 2*mm;
    G4Tubs *pmtGlassS = new G4Tubs("PMTGlass",0,pmtRadius,pmtThickness,0,360*deg);
    G4LogicalVolume* pmtGlassLV = new G4LogicalVolume(pmtGlassS,materials->GetMaterial("BK7"),"PMT Glass",0);

    // PMT Air
    G4Tubs *pmtAirS = new G4Tubs("PMTAir",0,2*pi,gs20Radius+refThickness,pmtRadius,gs20Thickness);
    G4LogicalVolume* pmtAirLV = new G4LogicalVolume(pmtAirS,materials->GetMaterial("G4_AIR"),"PMT Air Gap",0);
    
    // PMT Cap
    G4double capThickness = 2*mm;
    G4double capIRadius[3] = {0,gs20Thickness+refThickness,gs20Thickness+refThickness+capThickness};
    G4double capORadius[3] = {pmtRadius,pmtRadius,0};
    G4double capZPlane[3]  ={pmtRadius+capThickness,pmtRadius+capThickness,pmtRadius+capThickness};
    G4Polycone *pmtCapS = new G4Polycone("PMTCap",0,2*pi,3,capZPlane,capIRadius,capORadius);
    G4LogicalVolume* pmtCapLV = new G4LogicalVolume(pmtCapS,G4Material::GetMaterial("G4_POLYVINYL_CHLORIDE"),"PMT Cap",0);

    // Return the worlds physical volume
    return worldPV;
}

/**
 * SetSensitiveDetectors
 *
 * Setting the Sensitive Detectors of the Detector.
 * If the sensitive detectors exits, then only the senstive detector is 
 * registered to the logical volume.
 */
void DetectorConstruction::SetSensitiveDetectors(){
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    if (!pmtSD){
        pmtSD = new PMTSD("PMT/PMTSD","PMTHitCollection");
        SDman->AddNewDetector(pmtSD);
    }
    if (!absSD){
        absSD = new AbsorberSD("Absorber/AbsSD","AbsHitCollection");
        SDman->AddNewDetector(absSD);
    }
    gs20LV->SetSensitiveDetector(absSD);
    pmtLV->SetSensitiveDetector(pmtSD);
}
/**
 * SetVisAttributes()
 *
 * Sets the visualtion attributes
 */
#include "G4Colour.hh"
void DetectorConstruction::SetVisAttributes(){
    
    // Setting the Visualization attributes for the Abs
    {G4VisAttributes* atb= new G4VisAttributes(G4Colour::Cyan());
    //atb->SetForceWireframe(true);
    //atb->SetForceSolid(true);
    gs20LV->SetVisAttributes(atb);}

    // Setting the Layers to be white and invisiable
    {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
   // atb->SetForceWireframe(true);
    atb->SetVisibility(false);
    worldLV->SetVisAttributes(atb);}
    
    // Setting the World to be white and invisiable
    {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
    //atb->SetForceWireframe(true);
    atb->SetVisibility(false);
    worldPV->GetLogicalVolume()->SetVisAttributes(atb);}

}
/**
 * SetGS20Thickness
 *
 * Sets the detector thickness
 */
void DetectorConstruction::SetGS20Thickness(G4double val){
  gs20Thickness = val;
}

/**
 * SetGS20Radius
 *
 * Sets the calorimter radius
 */
void DetectorConstruction::SetGS20Radius(G4double val){
    if (val > pmtRadius){
        G4cout<<"Warning: tring to replace GS20 Radius with a radius that is bigger than the PMT"<<G4endl;
    } 
    else
      gs20Radius = val;
}

/**
 * UpdateGeometry
 *
 * Creates a new geometry, and reassings the sensitive detectors
 */
#include "G4RunManager.hh"
void DetectorConstruction::UpdateGeometry(){
    // Creating the new geomtry  
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
    
    // Setting the sensitive detectors
    SetSensitiveDetectors(); 

    // Updating the engine
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
