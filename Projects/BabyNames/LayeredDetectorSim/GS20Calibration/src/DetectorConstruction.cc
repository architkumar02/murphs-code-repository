#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "globals.hh"
#include "G4UnitsTable.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4RegionStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "Materials.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"


#include "G4SDManager.hh"
#include "AbsorberSD.hh"
#include "PMTSD.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"

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
    G4VPhysicalVolume* world = ConstructVolumes();

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
 * FindMaterial
 *
 * Finds, and if necessary, builds a material
 */
G4Material* DetectorConstruction::FindMaterial(G4String name){
  G4Material* material = G4Material::GetMaterial(name,true);
  return material;
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
    gs20Radius  = 1.27*cm;		      /* Radius of GS20 Sctintillator   */
    pmtRadius = 2.54*cm;
    pmtThickness = 2*mm;
    greaseThickness = 5*um;
    refThickness = 100*um;
    capThickness = 2*mm;                       /* Thickness of the cap     */
    absMaterial = FindMaterial("GS20");
    G4double capInSeam = gs20Thickness+refThickness;    /* Inside height of the cap */
    // World
    worldS = new G4Box("World",2.5*pmtRadius,2.5*pmtRadius,20*cm); 
    worldLV = new G4LogicalVolume(worldS,FindMaterial("G4_Galactic"),"World");
    worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",
            0,false,0,fCheckOverlaps);

    //
    // Setting up the GS20
    //
    // The beam is shotting along the z, comming from +z

    // GS20 Detector
    gs20S = new G4Tubs("Abs",0,gs20Radius,gs20Thickness/2,0,360*deg);
    gs20LV = new G4LogicalVolume(gs20S,absMaterial,"Absorber - GS20",0);
    gs20PV = new G4PVPlacement(0,G4ThreeVector(0,0,gs20Thickness/2.0),gs20LV,"Absorber - GS20",worldLV,false,0,fCheckOverlaps);
    
    // Light Reflector
    G4Tubs *refSide = new G4Tubs("refSide",gs20Radius,gs20Radius+refThickness,gs20Thickness,0,360*deg);
    G4Tubs *refTop = new G4Tubs("refTop",0,gs20Radius+refThickness,refThickness,0,360*deg);
    G4UnionSolid *refS = new G4UnionSolid("Reflector",refSide,refTop,0,G4ThreeVector(0,0,gs20Thickness));
    G4LogicalVolume* refLV = new G4LogicalVolume(refS,FindMaterial("G4_TEFLON"),"Reflector",0);
    G4VPhysicalVolume* refPV = new G4PVPlacement(0,G4ThreeVector(0,0,pmtThickness),refLV,"Reflector",worldLV,false,0,fCheckOverlaps);

    // Optical Grease
    G4Tubs *greaseS = new G4Tubs("opticalGrease",0,gs20Thickness,greaseThickness,0,360*deg);
    G4LogicalVolume* greaseLV = new G4LogicalVolume(greaseS,FindMaterial("Silicone"),"PMT Glass",0);
    G4VPhysicalVolume* greasePV = new G4PVPlacement(0,G4ThreeVector(0,0,-greaseThickness),greaseLV,"Grease",worldLV,false,0,fCheckOverlaps);
  
    // PMT Glass
    G4Tubs *pmtS = new G4Tubs("PMTGlass",0,pmtRadius,pmtThickness,0,360*deg);
    pmtLV = new G4LogicalVolume(pmtS,FindMaterial("BK7"),"PMT Glass",0);
    pmtPV = new G4PVPlacement(0,G4ThreeVector(0,0,-1*(pmtThickness+2*greaseThickness)),pmtLV,"PMTGlass",worldLV,false,0,fCheckOverlaps);
    
    // PMT Cap
    G4Tubs *capSide = new G4Tubs("CapSide",pmtRadius,pmtRadius+capThickness,capInSeam,0,2*pi);
    G4Tubs *capTop = new G4Tubs("CapTop",0,pmtRadius+capThickness,capThickness,0,2*pi);
    G4UnionSolid *pmtCapS = new G4UnionSolid("PMTCap",capSide,capTop,0,G4ThreeVector(0,0,capInSeam));
    G4LogicalVolume* pmtCapLV = new G4LogicalVolume(pmtCapS,FindMaterial("G4_POLYVINYL_CHLORIDE"),"PMT Cap",0);
    G4VPhysicalVolume* pmtCapPV = new G4PVPlacement(0,G4ThreeVector(0,0,0),pmtCapLV,"PMTCap",worldLV,false,0,fCheckOverlaps);

    // PMT Air
    G4Tubs *pmtAirS = new G4Tubs("PMTAir",0,2*pi,gs20Radius+refThickness,pmtRadius,gs20Thickness);
    G4LogicalVolume* pmtAirLV = new G4LogicalVolume(pmtAirS,FindMaterial("G4_AIR"),"PMT Air Gap",0);
  
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
void DetectorConstruction::UpdateGeometry(){

    if(!worldPV) return;
  
    // Cleaning up previous geometry
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4LogicalSkinSurface::CleanSurfaceTable();
    G4LogicalBorderSurface::CleanSurfaceTable();

    // Creating the new geomtry  
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
    SetSensitiveDetectors(); 
    
    // Updating the engine
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    G4RegionStore::GetInstance()->UpdateMaterialList(worldPV);
}
