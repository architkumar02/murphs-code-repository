#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

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

/**
 * DetectorConstruction
 *
 * Setting the default detector construciton.
 */
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
    fCheckOverlaps(true){

        // Geometry parameters
        sliceThickness= 5*um;           // Thickness of a slice
        absThickness = 50*um;	        // Thickness of Absorber

        gapThickness = 1*cm;            // Thickness of Gap 
        oRadius  = 2.54*cm;		        // Outer Radius of Detector
        iRadius = 0.*cm;				// Inner radious of  Detector
        startAngle = 0.*deg;
        spanAngle = 360.*deg;

        // Compute parameters
        ComputeParameters();

        // Define materials 
        DefineMaterials();
        SetAbsorberMaterial("EJ426HD2");
        SetGapMaterial("G4_PLEXIGLASS");

        // Create commands for interactive defiantions of the calorimeter
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
    G4VPhysicalVolume* calo = ConstructCalorimeter();

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

    // Defining EJ426 HD2
    G4double massFraction;
    G4Material* EJ426HD2 = new G4Material("EJ426HD2",density=4.1*g/cm3,nComponents=4);
    EJ426HD2->AddElement(enrichLi,massFraction=0.081);
    EJ426HD2->AddElement(eF,massFraction=0.253);
    EJ426HD2->AddElement(eZn,massFraction=0.447);
    EJ426HD2->AddElement(eS,massFraction=0.219);

    // Vacuum
    new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);

    nistManager->FindOrBuildMaterial("G4_PLEXIGLASS",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_AIR",fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_WATER",fromIsotopes);
    // Print materials
    //G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    // Get materials
    defaultMaterial = G4Material::GetMaterial("Galactic");
}


/**
 * ComputeParameters
 *
 */
void DetectorConstruction::ComputeParameters(){

    caloThickness = absThickness+2*gapThickness;
    worldSizeXY = 1.2 * oRadius;
    worldSizeZ  = caloThickness+2*cm; 

        
    numberAbsSlices = ceil(absThickness/sliceThickness);
    numberGapSlices = ceil(gapThickness/sliceThickness);
}

/**
 * PrintCaloParameters
 *
 * Prints the parameters of the geometry
 */
void DetectorConstruction::PrintCaloParameters(){

    // print parameters
    G4cout << "\n------------ Calorimeter Parameters ---------------------"
        <<"\n--> The carlorimeter is a single layer of: \n\t[ "
        << absThickness/um << "um of " << absMaterial->GetName() 
        << " + "
        << gapThickness/mm << "mm of " << gapMaterial->GetName() << " ]"
        << "\n--> A single slice is " <<sliceThickness/um << " um thick."
        << "\n--> There are "<<2*numberGapSlices<<" in the gap (left and right)."
        << "\n--> There are "<<numberAbsSlices<<" in the abosrber."
        << "\n--> The calormeter is " <<caloThickness/cm << " cm thick"
        << " with a radius of "<<oRadius/cm<<" cm"
        << "\n------------------------------------------------------------\n"
        <<" The world is "<<worldSizeXY/cm<<" cm by "<<worldSizeXY/cm 
        <<" cm by "<<worldSizeZ/cm<<" cm."
        <<G4endl;
}

/**
 * ConstructCalorimeter()
 *
 * Calorimeter is constructed as a solid cylinder of the gap (non-scintillating)
 * material with layers of the absorber (scintillating) material.
 */
G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter(){

    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    // complete the Calor parameters definition
    ComputeParameters();


    // World
    worldS = new G4Box("World",worldSizeXY, worldSizeXY, worldSizeZ*0.5); 
    worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");
    worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",
            0,false,0,fCheckOverlaps);

    //
    // Setting up the Calorimeter
    //

    // Absorber
    absS = new G4Tubs("Abs",iRadius,oRadius,absThickness/2,0,spanAngle);
    absLV = new G4LogicalVolume(absS,absMaterial,"Absorber",0);
    absPV = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,0.0),
                absLV,"Absorber",worldLV,false,0,fCheckOverlaps);
    absSSlice = new G4Tubs("AbsSlice",iRadius,oRadius,sliceThickness/2,0,spanAngle);
    absLVSlice = new G4LogicalVolume(absSSlice,absMaterial,"Absorber",0);
    absPVSlice = new G4PVReplica("AbsSlicedPV",absLVSlice,absLV,kZAxis,numberAbsSlices,sliceThickness);

    // Gap (right)
    G4double offset = (gapThickness + absThickness)/2;
    gapRS = new G4Tubs("Gap Right",iRadius,oRadius,gapThickness/2,0,spanAngle);
    gapRLV = new G4LogicalVolume(gapRS,gapMaterial,"Gap",0);
    gapRPV = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,-offset),
                gapRLV,"Gap Right",worldLV,false,0,fCheckOverlaps);
    gapRSSlice = new G4Tubs("GapSlice",iRadius,oRadius,sliceThickness/2,0,spanAngle);
    gapRLVSlice = new G4LogicalVolume(gapRSSlice,gapMaterial,"Gap Right",0);
    gapRPVSlice = new G4PVReplica("GapSlicedPV",gapRLVSlice,gapRLV,kZAxis,numberGapSlices,sliceThickness);
    
    // Gap (left)
    gapLS = new G4Tubs("Gap Left",iRadius,oRadius,gapThickness/2,0,spanAngle);
    gapLLV = new G4LogicalVolume(gapLS,gapMaterial,"Gap",0);
    gapLPV = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,offset),
                gapLLV,"Gap Left",worldLV,false,0,fCheckOverlaps);
    gapLSSlice = new G4Tubs("GapSlice",iRadius,oRadius,sliceThickness/2,0,spanAngle);
    gapLLVSlice = new G4LogicalVolume(gapLSSlice,gapMaterial,"Gap Left",0);
    gapLPVSlice = new G4PVReplica("GapSlicedPV",gapLLVSlice,gapLLV,kZAxis,numberGapSlices,sliceThickness);
       
    PrintCaloParameters();

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
    absSD = new CaloSensitiveDetector("SD/AbsSD","AbsHitCollection");
    SDman->AddNewDetector(absSD);
    absLV->SetSensitiveDetector(absSD);

    gapSD = new CaloSensitiveDetector("SD/GapSD","GapHitCollection");
    SDman->AddNewDetector(gapSD);
    gapLLV->SetSensitiveDetector(gapSD);
    gapRLV->SetSensitiveDetector(gapSD);

    // Setting the Maximum Step Size
    G4double maxStep = 0.5*absThickness;
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

    // Setting the Visualization attributes for the Gap
    {G4VisAttributes* atb= new G4VisAttributes(G4Colour::Gray());
    //atb->SetForceWireframe(true);
    //atb->SetForceSolid(true);
    gapLLV->SetVisAttributes(atb);
    gapRLV->SetVisAttributes(atb);}

    // Setting the Layers to be white and invisiable
    {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
    atb->SetForceWireframe(true);
    atb->SetVisibility(false);
    worldLV->SetVisAttributes(atb);}
    
    // Setting the World to be white and invisiable
    {G4VisAttributes* atb = new G4VisAttributes(G4Colour::White());
    atb->SetForceWireframe(true);
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
void DetectorConstruction::SetCaloRadius(G4double val){
    oRadius = val;
}

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
    
    absLV->SetSensitiveDetector(absSD);
    gapLLV->SetSensitiveDetector(gapSD);
    gapRLV->SetSensitiveDetector(gapSD);

    // Setting the Maximum Step Size
    G4double maxStep = 0.5*absThickness;
    absLV->SetUserLimits(new G4UserLimits(maxStep));

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
