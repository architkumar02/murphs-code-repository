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

#include "Analysis.hh"

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

DetectorConstruction::~DetectorConstruction(){

}

/**
 * Construct
 *
 * Creating the detector's world volume
 */
G4VPhysicalVolume* DetectorConstruction::Construct(){

    // Return Physical World
    G4VPhysicalVolume* calo = Construct();

    // Set Visulation Attributes
    SetVisAttributes();

    // Assign Sensitve Detectors
    SetSensitiveDetectors();
    return calo;
}


void DetectorConstruction::DefineMaterials()
{
    G4String name, symbol;             // a=mass of a mole;
    G4double a, z, density;            // z=mean number of protons;  
    G4int iz, n;                       // iz=nb of protons  in an isotope; 
    // n=nb of nucleons in an isotope;
    G4int nAtoms;
    G4int nComponents;

    // NIST Material Manager
    G4NistManager* nistManager = G4NistManager::Instance();
    G4bool fromIsotopes = true;

    // Getting Elements
    G4Element* eH = nistManager->FindOrBuildElement("H",fromIsotopes);
    G4Element* eC = nistManager->FindOrBuildElement("C",fromIsotopes);
    G4Element* eO = nistManager->FindOrBuildElement("O",fromIsotopes);
    G4Element* eN = nistManager->FindOrBuildElement("N",fromIsotopes);
    G4Element* eF = nistManager->FindOrBuildElement("F",fromIsotopes);
    G4Element* eS = nistManager->FindOrBuildElement("S",fromIsotopes);
    G4Element* eZn = nistManager->FindOrBuildElement("Zn",fromIsotopes);

    // defining enriched Lithium 6
    G4double a6Li = 6.015*g/mole;	// Molar Masses (Wolfram Alpha)
    G4double a7Li = 7.016*g/mole;
    G4double enrichement = 110.815*perCent;
    G4double abundance6Li = enrichement*a6Li/a7Li;		// Relative Abudadance
    G4double abundance7Li = 1-abundance6Li;

    G4Isotope* Li6 = new G4Isotope(name="Li6", iz=3, n=6, a6Li);
    G4Isotope* Li7 = new G4Isotope(name="Li7", iz=3, n=7, a7Li);
    G4Element* enrichLi  = new G4Element(name="enriched Lithium", symbol="Li", nComponents=2);
    enrichLi->AddIsotope(Li6, abundance6Li);
    enrichLi->AddIsotope(Li7, abundance7Li);

    // Defining 6LiF Absorber
    G4Material* LiAbsorber = new G4Material("6LiF",density=1.0*g/cm3,nComponents=2);
    LiAbsorber->AddElement(enrichLi,nAtoms=1);
    LiAbsorber->AddElement(eF,nAtoms=1);


    nistManager->FindOrBuildMaterial("G4_PLEXIGLASS",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_AIR",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_WATER",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_TEFLON",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_Galactic",fromIsotopes);
   
   // Print materials
    // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
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
    gs20PV = new G4PVPlacement(0,G4ThreeVector(0,0,0),
                absLV,"Absorber",worldLV,false,0,fCheckOverlaps);
    
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

    // PMT Glass
    G4Tubs *pmtGlass = new G4Tubs("PMTGlass",0,pmtRadius,pmtThickness,0,360*deg);

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
