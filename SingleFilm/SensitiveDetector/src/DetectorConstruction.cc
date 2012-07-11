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


/**
 * DetectorConstruction
 *
 * Setting the default detector construciton.
 */
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
	fCheckOverlaps(true){

		// Geometry parameters
		absThickness = 50.*um;	      // Thickness of Absorber
		gapThickness =  0.3175*cm;    // Thickness of Gap 
		oRadius  = 2.54*cm;		   // Outer Radius of Detector
		iRadius = 0.*cm;				// Inner radious of  Detector
		startAngle = 0.*deg;
		spanAngle = 360.*deg;

		nofLayers = 10;              // Number of detector layers

		// Compute parameters
		ComputeParameters();

		// Define materials 
		DefineMaterials();
		SetAbsorberMaterial("PS_Detector");
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


	// Create the Detector
	ConstructCalorimeter();

	// Set Visulation Attributes
	SetVisAttributes();

	// Assign Sensitve Detectors
	SetSensitiveDetectors();

	// Return Physical World
	return ConstructCalorimeter();
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
	//G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	// Get materials
	defaultMaterial = G4Material::GetMaterial("Galactic");
}


/**
 * ComputeParameters
 *
 */
void DetectorConstruction::ComputeParameters(){

	layerThickness = absThickness + gapThickness;
	caloThickness = layerThickness*nofLayers;
	worldSizeXY = 1.2 * oRadius;
	worldSizeZ  = 1.2 * caloThickness; 
}
/**
 * PrintCaloParameters
 *
 * Prints the parameters of the geometry
 */
void DetectorConstruction::PrintCaloParameters(){

	// print parameters
	G4cout << "\n------------ Calorimeter Parameters ---------------------"
		<<"\n--> The carlorimeter is "<< nofLayers << " layers of: \n\t[ "
		<< absThickness/mm << "mm of " << absMaterial->GetName() 
		<< " + "
		<< gapThickness/mm << "mm of " << gapMaterial->GetName() << " ]"
		<< "\n--> A single layer is " <<layerThickness/cm << " cm thick."
		<< "\n--> The calormeter is " <<caloThickness/cm << " cm thick"
		<< " with a radius of "<<oRadius/cm<<" cm"
		<< "\n------------------------------------------------------------\n"
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
	worldS = new G4Box("World",worldSizeXY, worldSizeXY, worldSizeZ); 
	worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");
	worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",
                0,false,0,fCheckOverlaps);

	//
	// Setting up the Calorimeter
	//
	caloS = 0; caloLV = 0; caloPV = 0;
	layerS = 0; layerLV = 0; layerPV = 0;

	// Calorimeter (gap material)
	caloS = new G4Tubs("Calorimeter",iRadius,oRadius,caloThickness/2,
			startAngle,spanAngle);
	caloLV = new G4LogicalVolume(caloS,gapMaterial,
			"Calorimeter");
	caloPV = new G4PVPlacement(0,G4ThreeVector(0,0,-caloThickness/2),
			caloLV,"Calorimeter",worldLV,false,0,fCheckOverlaps);

	// Layer (Consists of Absorber and Gap)
	layerS = new G4Tubs("Layer",iRadius,oRadius,layerThickness/2,
			startAngle,spanAngle);
	layerLV = new G4LogicalVolume(layerS,defaultMaterial,"Layer");
	if (nofLayers > 1){
		layerPV = new G4PVReplica("Layer",layerLV,caloLV,kZAxis,
					nofLayers,layerThickness);
	}else{
		layerPV = new G4PVPlacement(0,G4ThreeVector(),layerLV,"Layer",
					caloLV,false,0);
	}

	// Absorber
	absS = 0; absLV = 0; absPV = 0;
	if (absThickness > 0.0){
		absS = new G4Tubs("Abso",iRadius,oRadius,absThickness/2,
				startAngle,spanAngle);
		absLV = new G4LogicalVolume(absS,absMaterial,absMaterial->GetName(),0);
		absPV = new G4PVPlacement(0,G4ThreeVector(-gapThickness/2,0.0,0.0),
				absLV,absMaterial->GetName(),layerLV,false,0);
	}

	// Gap
	gapS = 0; gapLV = 0; gapPV = 0;
	if (gapThickness > 0.0){
		gapS = new G4Tubs("Gap",iRadius,oRadius,gapThickness/2,
				startAngle,spanAngle);
		gapLV = new G4LogicalVolume(gapS,gapMaterial,
				gapMaterial->GetName(),0);
		gapPV = new G4PVPlacement(0,G4ThreeVector(-gapThickness/2,0.0,0.0),
				gapLV,gapMaterial->GetName(),layerLV,false,0);
	}

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
	caloSD = new CaloSensitiveDetector("AbsorberSD");
	SDman->AddNewDetector(caloSD);
	absLV->SetSensitiveDetector(caloSD);

}
/**
 * SetVisAttributes()
 *
 * Sets the visualtion attributes
 */
#include "G4Colour.hh"
void DetectorConstruction::SetVisAttributes(){
  // Below are vis attributes that permits someone to test / play 
  // with the interactive expansion / contraction geometry system of the
  // vis/OpenInventor driver :
 {G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  simpleBoxVisAtt->SetVisibility(true);
  delete caloLV->GetVisAttributes();
  caloLV->SetVisAttributes(simpleBoxVisAtt);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  layerLV->SetVisAttributes(atb);}
  
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  atb->SetForceSolid(true);
  absLV->SetVisAttributes(atb);}
  
 {//Set opacity = 0.2 then transparency = 1 - 0.2 = 0.8
  G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.2));
  atb->SetForceSolid(true);
  gapLV->SetVisAttributes(atb);}
	
	/*
	G4VisAttributes* absVisAtt = new G4VisAttributes(G4Colour::Blue());
	absVisAtt->SetVisibility(true);
	caloPV->GetLogicalVolume()->SetVisAttributes(absVisAtt);
*/
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
/**
 * SetNumberOfLayers
 *
 * Sets the number of layers in the detector
 */
void DetectorConstruction::SetNumberOfLayers(G4int val){
	nofLayers = val;
}

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
	G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
}
