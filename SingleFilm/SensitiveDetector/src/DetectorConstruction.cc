#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"

#include "CaloSensitiveDetector.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"



DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
    fCheckOverlaps(true){

	// Define materials 
	DefineMaterials();
}


DetectorConstruction::~DetectorConstruction(){
 
}


G4VPhysicalVolume* DetectorConstruction::Construct(){
	
	// World
	G4VSolid* worldS = new G4Box("World",worldSizeXY, worldSizeXY, worldSizeZ); 
	worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");
	G4VPhysicalVolume* worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);
	
    // Create the Detector
    ConstructCalorimeter();

    // Set Visulation Attributes
    SetVisAttributes();

    // Assign Sensitve Detectors
    SetSensitiveDetectors();

    // Return Physical World
    return worldPV;
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


	// PPO C15H11NO
	G4Material* PPO = new G4Material("PPO",density=1.1*g/cm3,nComponents=4,kStateSolid);
	PPO->AddElement(eC,nAtoms=15);
	PPO->AddElement(eH,nAtoms=11);
	PPO->AddElement(eO,nAtoms=1);
	PPO->AddElement(eN,nAtoms=1);

	// POPOP C24H15N2O2
	G4Material* POPOP = new G4Material("POPOP",density=1.1*g/cm3,nComponents=4,kStateSolid);
	POPOP->AddElement(eC,nAtoms=24);
	POPOP->AddElement(eH,nAtoms=15);
	POPOP->AddElement(eO,nAtoms=2);
	POPOP->AddElement(eN,nAtoms=2);

	// Scintillant
	G4double fractionPPO = 46./(46.+1.36);		// Scintillant is in the ratio of 46 g PPO to 1.36 g POPOP
	G4Material* scintillant = new G4Material("PPO/POPOP",density=1.1*g/cm3,nComponents=2,kStateSolid);
	scintillant->AddMaterial(PPO,fractionPPO);
	scintillant->AddMaterial(POPOP,1-fractionPPO);

	// Polymer PS Based Detector
	G4double fractionPolymer = 0.65;
	G4double fractionScintillant = 0.05;
	G4double fractionAbsorber = 0.30;
	G4Material* psDetector = new G4Material("PS_Detector",density=1.1*g/cm3,nComponents=3,kStateSolid);
	psDetector->AddMaterial(nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes),fractionPolymer);
	psDetector->AddMaterial(scintillant,fractionScintillant);
	psDetector->AddMaterial(LiAbsorber,fractionAbsorber);

	// Vacuum
	new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);

	nistManager->FindOrBuildMaterial("G4_PLEXIGLASS",fromIsotopes);
	nistManager->FindOrBuildMaterial("G4_AIR",fromIsotopes);
	
	// Print materials
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	// Get materials
	defaultMaterial = G4Material::GetMaterial("Galactic");
	absorberMaterial = G4Material::GetMaterial("PS_Detector");
	gapMaterial = G4Material::GetMaterial("G4_PLEXIGLASS");
	
	if ( ! defaultMaterial || ! absorberMaterial || ! gapMaterial ) {
		G4cerr << "Cannot retrieve materials already defined. " << G4endl;
		G4cerr << "Exiting application " << G4endl;
		exit(1);
	}  
}

void DetectorConstruction::ComputeParameters(){

	// Geometry parameters
	absoThickness = 50.*um;	        // Thickness of Absorber
	gapThickness =  0.3175*cm;      // Thickness of Gap 
	outerRadius  = 2.54*cm;		    // Outer Radius of Detector
	innerRadius = 0.*cm;			// Inner radious of  Detector
	startAngle = 0.*deg;
	spanAngle = 360.*deg;
	
    nofLayers = 1;                  // Number of detector layers
    layerThickness = absoThickness + gapThickness;
	caloThickness = layerThickness*nofLayers;
    worldSizeXY = 1.2 * outerRadius;
    worldSizeZ  = 1.2 * calorThickness; 
	
    // print parameters
	G4cout << "\n------------------------------------------------------------"
	<<"\n---> The carlorimeter is "<< noLayers << " layers of: [ "
    << absoThickness/mm << "mm of " << absorberMaterial->GetName() 
	<< " + "
	<< gapThickness/mm << "mm of " << gapMaterial->GetName() << " ] \n"
	<< "\n---> A single layer is " <<layerThickness/cm << "cm thick."
    << "\n---> The calormeter is " <<caloThickness/cm << "cm thick"
	<<" with a radius of"<<outerRadius/cm<<" cm"
	<< "\n------------------------------------------------------------\n";
}

/**
 * ConstructCalorimeter()
 *
 * Calorimeter is constructed as a solid cylinder of the gap (non-scintillating)
 * material with layers of the absorber (scintillating) material.
 */
G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter(){
    G4int caloCopyNum = 1000;

    // Calorimeter (gap material)
    G4Tubs* caloS = new G4Tubs("Calorimeter",innerRadius,outerRadius,
                        caloThickness/2,startAngle,spanAngle);
    G4LogicalVolume* caloLV = new G4LogicalVolume(caloS,gapMaterial,
                        "Calorimeter");
    caloPV = new G4VPlacement(0,G4ThreeVector(0,0,-caloThickness/2),
                caloLV,"Calorimeter",worldLV,false,caloCopyNum,fCheckOverlaps);

	// Absorber
	G4Tubs* absorberS = new G4Tubs("Abso",innerRadius,outerRadius,
                        absoThickness/2,startAngle,spanAngle);
	G4LogicalVolume* absorberLV = new G4LogicalVolume(absorberS,
                        absorberMaterial,"Abso",0);
	
    G4ThreeVector gapLayer(0,0,gapThickness);
    G4ThreeVector actLayer(0,0,absThickness);
    G4int layerCopyNum = caloCopyNum;
    for(int layerIdx = 0; layerIdx < nofLayers; ++layerIdx){
        // Calculate the new position
        G4ThreeVector position = (layerIdx+1)*gapLayer+(layerIdx+0.5)*actLayer;
        position -= G4ThreeVector(0,0,caloThickness/2);
        
        // Place the Detector
        new G4PVPlacement(0,position,absorberLV,"Absorber",caloLV,false,
            ++layerCopyNum,fCheckOverlaps);
    }

	
    // Return calorimeter physical volume
	return caloPV;
}

void DetectorConstruction::SetSensitiveDetecotrs(){

	// 
	// Scorers
	//
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	TrackerSD* caloSD = new TrackerSD("AbsorberSD");		// Absorber SD
	SDman->AddNewDetector(absoSD);
	absorberLV->SetSensitiveDetector(absoSD);

}

void DetectorConstruction::SetVisAttributes(){

	//                                        
	// Visualization attributes
	//
	//worldLV->SetVisAttributes (G4VisAttributes::Invisible);
	//G4VisAttributes* gapVisAtt = new G4VisAttributes(G4Color(1,0,0,0.5));
	//gapVisAtt->SetVisibility(true);
	//gapLV->SetVisAttributes(gapVisAtt);
	//G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	//simpleBoxVisAtt->SetVisibility(true);
	//calorLV->SetVisAttributes(simpleBoxVisAtt);
	

}
