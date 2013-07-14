#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"


#include "CaloSensitiveDetector.hh"
#include "G4SDManager.hh"

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

        worldSizeZ  = 25*cm;      // Fixed World Size; 
        
        // Geometry parameters
        gs20Thickness = 2*mm;	        // Thickness of Absorber
        gs20Radius  = 2.54*cm;		        // Outer Radius of Detector
        iRadius = 0.*cm;				// Inner radious of  Detector
        startAngle = 0.*deg;
        spanAngle = 360.*deg;

        // Define materials and properties 
        DefineMaterials();
        DefineOpticalMaterialProperties();
        DefineOpticalSurfaces();
        
        // Create commands for interactive defiantions of the calorimeter
        SetDetectorMaterial("GS20");
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
    return wold;
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
G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter(){

    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    // World
    worldS = new G4Box("World",2.5*pmtRadius,2.5*pmtRadius,20*cm); 
    worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");
    worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",
            0,false,0,fCheckOverlaps);

    //
    // Setting up the GS20
    //
    // The beam is shotting along the z, comming from +z

    // GS20 Detector
    gs20S = new G4Tubs("Abs",0,gs20Radius,gs20Thickness/2,0,360*deg);
    gs20LV = new G4LogicalVolume(absS,absMaterial,"Absorber",0);
    gs20PV = new G4PVPlacement(0,G4ThreeVector(0,0,0),absLV,"Absorber",worldLV,false,0,fCheckOverlaps);
    
    // Light Reflector
    G4Double rInner = {gs20Radius,gs20Radius,0};
    G4Double rOuter = {pmtRadius,gs20Radius+refThickness,gs20Radius+refThickness};
    G4double zPlane  = {refThickness,gs20Thickness-refThickness,gs20Thickness+refThickness};
    G4Polycone *refS = new G4Polycone("Ref",0,2*Pi,zPlane,rInner,rOuter);
    /*
    G4Tubs *refFlange = new G4Tubs("refFlange",gs20Radius,pmtRadius,refThickness,0,360*deg);
    G4Tubs *refSide = new G4Tubs("refSide",gs20Radius,gs20Radius+refThickness,gs20Thickness,0,360*deg);
    G4Tubs *refTop = new G4Tubs("refTop",0,gs20Radius+refThickness,refThickness,0,360*deg);
    */

    // Optical Grease
    G4double greaseThickenss = 5*um;
    G4Tubs *greaseS = new G4Tubs("opticalGrease",0,gs20Thickness,greaseThickness,0,360*deg);
    G4LogicalVolume* pmtGlassL = new G4LogicalVolume(pmtGlassS,G4Material::GetMaterial("OpticalGrease"),"PMT Glass",0);

    // PMT Glass
    G4Tubs *pmtGlassS = new G4Tubs("PMTGlass",0,pmtRadius,pmtThickness,0,360*deg);
    G4LogicalVolume* pmtGlassL = new G4LogicalVolume(pmtGlassS,G4Material::GetMaterial("Borosilicate Glass"),"PMT Glass",0);

    // PMT Air
    G4Tubs *pmtAirS = new G4Tubs("PMTAir",0,2*Pi,gs20Radius+refThickness,pmtRadius,gs20Thicknss);
    G4LogicalVolume* pmtAirL = new G4LogicalVolume(pmtAirS,G4Material::GetMaterial("G4_AIR"),"PMT Air Gap",0);
    
    // PMT Cap
    G4double capThickness = 2*mm;
    G4double capIRadius = {0,gs20Thickness+refThickness,gs20Thickness+refThicknss+capThickness};
    G4double capORadius = {pmtRadius,pmtRadius,0};
    G4double capZPlane  ={pmtRadius+capThickness,pmtRadius+capThickness,pmtRadius+capThickness};
    G4Polycone *pmtCapS = new G4Polycone("PMTCap",0,2*Pi,capZPlane,capIRadius,capORadius);
    G4LogicalVolume* pmtCapL = new G4LogicalVolume(pmtCapS,G4Material::GetMaterial("G4_POLYVINYL_CHLORIDE"),"PMT Cap",0);

    // Return the worlds physical volume
    return worldPV;
}
/*
 * DefineOpticalProperties
 *
 * Defines the optical properties of the materials used in the simulation
 */
void DetectorConstruction::DefineOpticalProperties(){
	/*
   * set optical properties of Boroscilate Glass
   */
	G4MaterialPropertiesTable* propertiesTable = new G4MaterialPropertiesTable();

	// set absorption length
	G4MaterialPropertyVector* absorption = new G4MaterialPropertyVector();
	absorption->AddElement(0.1 * CLHEP::eV, 0.1 * CLHEP::mm);
	absorption->AddElement(10. * CLHEP::eV, 0.1 * CLHEP::mm);
	propertiesTable->AddProperty("ABSLENGTH", absorption);

	// set refractive index
	G4MaterialPropertyVector* rindex = new G4MaterialPropertyVector();
	rindex->AddElement(0.1 * CLHEP::eV, 1.54);
	rindex->AddElement(10. * CLHEP::eV, 1.54);
	propertiesTable->AddProperty("RINDEX", rindex);

	G4Material::GetMaterial("Borosilicate Glass", true)->SetMaterialPropertiesTable(propertiesTable);

}

/**
 * SetSensitiveDetectors
 *
 * Setting the Sensitive Detectors of the Detector
 */
void DetectorConstruction::SetSensitiveDetectors(){
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    caloSD = new CaloSensitiveDetector("SD/CaloSD","CaloHitCollection");
    SDman->AddNewDetector(caloSD);
    absLV->SetSensitiveDetector(caloSD);

    // Setting the Maximum Step Size
    G4double maxStep = 0.01*absThickness;
    absLV->SetUserLimits(new G4UserLimits(maxStep));
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
    absLV->SetVisAttributes(atb);}

    // Setting the Visualization attributes for the Calorimeter 
    {G4VisAttributes* atb= new G4VisAttributes(G4Colour::Gray());
    //atb->SetForceWireframe(true);
    //atb->SetForceSolid(true);
    gapLV->SetVisAttributes(atb);}

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
 * SetAbsorberMaterial
 *
 * Set's the absorber material, which contains the neutron absorber
 */
void DetectorConstruction::SetAbsorberMaterial(G4String materialChoice){
    G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
    if (pttoMaterial) absMaterial = pttoMaterial;
}
/**
 * SetGapMaterial
 *
 * Set's the gap material.
 */
void DetectorConstruction::SetGapMaterial(G4String materialChoice){
    G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
    if (pttoMaterial) gapMaterial = pttoMaterial;
}

/**
 * SetAbsorberThickness
 *
 * Sets the absorber thickness
 */
void DetectorConstruction::SetAbsorberThickness(G4double val){
    absThickness = val;
}

/**
 * SetGapThickness
 *
 * Sets the thickness of the gap
 */
void DetectorConstruction::SetGapThickness(G4double val){
    gapThickness = val;
}

/**
 * SetCaloRadius
 *
 * Sets the calorimter radius
 */
void DetectorConstruction::SetGS20Radius(G4double val){
    gs20Radius = val;
}

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
    
    absLV->SetSensitiveDetector(caloSD);

    // Setting the Maximum Step Size
    G4double maxStep = 0.01*absThickness;
    absLV->SetUserLimits(new G4UserLimits(maxStep));

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
