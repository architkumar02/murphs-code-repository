#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "TrackerSD.hh"
#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSFlatSurfaceFlux.hh"
#include "G4PSNofSecondary.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include <stdio.h>


DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
fMessenger(this),
fCheckOverlaps(true)
{
}


DetectorConstruction::~DetectorConstruction(){
 
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Define materials 
	DefineMaterials();
	
	// Define volumes
	return DefineVolumes();
	
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

}


G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
	// Geometry parameters
	G4double absoThickness = 50.*um;	    // Thickness of Absorber (Scintilating)
	G4double gapThickness =  0.3175*cm;  // Thickness of Gap (Non-Scintilating)
	G4double outerRadius  = 2.54*cm;		 // Outer Radius of Detector
	G4double innerRadius = 0.*cm;			 // Inner radious of  Detector
	G4double startAngle = 0.*deg;
	G4double spanAngle = 360.*deg;
	
	G4double layerThickness = absoThickness + gapThickness;
	G4double worldSizeXY = 1.2 * outerRadius;
	G4double worldSizeZ  = 1.2 * layerThickness; 
	
	// Get materials
	G4Material* defaultMaterial = G4Material::GetMaterial("Galactic");
	G4Material* absorberMaterial = G4Material::GetMaterial("PS_Detector");
	G4Material* gapMaterial = G4Material::GetMaterial("G4_PLEXIGLASS");
	
	if ( ! defaultMaterial || ! absorberMaterial || ! gapMaterial ) {
		G4cerr << "Cannot retrieve materials already defined. " << G4endl;
		G4cerr << "Exiting application " << G4endl;
		exit(1);
	}  
	
	// World
	G4VSolid* worldS = new G4Box("World",worldSizeXY, worldSizeXY, worldSizeZ); 
	G4LogicalVolume* worldLV = new G4LogicalVolume(worldS,defaultMaterial,"World");
	G4VPhysicalVolume* worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"World",0,false,0,fCheckOverlaps);
	
	// Absorber
	G4Tubs* absorberS = new G4Tubs("Abso",innerRadius,outerRadius,absoThickness/2,startAngle,spanAngle);
	G4LogicalVolume* absorberLV = new G4LogicalVolume(absorberS,absorberMaterial,"Abso");
	new G4PVPlacement(0,G4ThreeVector(0., 0.,-gapThickness/2),absorberLV,"Abso",worldLV,false,0,fCheckOverlaps);
	
	// Gap
	G4Tubs* gapS = new G4Tubs("Layer",innerRadius,outerRadius,gapThickness/2,startAngle,spanAngle);
	G4LogicalVolume* gapLV= new G4LogicalVolume(gapS,gapMaterial,"Gap");
	new G4PVPlacement(0,G4ThreeVector(0., 0.,absoThickness/2),gapLV,"Gap",worldLV,false,0,fCheckOverlaps); 
	
	// print parameters
	G4cout << "\n------------------------------------------------------------"
	<< absoThickness/mm << "mm of " << absorberMaterial->GetName() 
	<< " + "
	<< gapThickness/mm << "mm of " << gapMaterial->GetName() << " ] \n"
	<< "---> Deteoctr is "<<layerThickness/cm
	<<" cm thick with a radius of"<<outerRadius/cm<<" cm"
	<< "\n------------------------------------------------------------\n";
	
	
	// 
	// Scorers
	//
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	TrackerSD* absoSD = new TrackerSD("AbsorberSD");		// Absorber SD
	SDman->AddNewDetector(absoSD);
	absorberLV->SetSensitiveDetector(absoSD);
	
	TrackerSD* gapSD = new TrackerSD("GapSD");				// Gap SD
	SDman->AddNewDetector(gapSD);
	gapLV->SetSensitiveDetector(gapSD);
	
	
	// declare Absorber as a MultiFunctionalDetector scorer
	G4MultiFunctionalDetector* absDetector = new G4MultiFunctionalDetector("Absorber");
	G4SDManager::GetSDMpointer()->AddNewDetector(absDetector);
	absorberLV->SetSensitiveDetector(absDetector);
	
	// Setting up Scorers
	G4VPrimitiveScorer* primitive;
	G4SDChargedFilter* charged = new G4SDChargedFilter("chargedFilter");
	primitive = new G4PSEnergyDeposit("Edep");
	absDetector->RegisterPrimitive(primitive);
	
	primitive = new G4PSTrackLength("TrackLength");
	primitive ->SetFilter(charged);
	absDetector->RegisterPrimitive(primitive);  
	
	// Flux and Dose Scores
	// O is in || out, 1 is in, 2 is out
	primitive = new G4PSFlatSurfaceFlux("TotalSurfaceFlux",1);
	absDetector->RegisterPrimitive(primitive);
	primitive = new G4PSDoseDeposit("TotalDose");
	absDetector->RegisterPrimitive(primitive);
	
	// Number of Secondaries
	G4SDParticleFilter* gammaFilter = new G4SDParticleFilter("gammaFilter","gamma");
	gammaFilter->add("opticalphoton");
	G4SDParticleFilter* electronFilter = new G4SDParticleFilter("electronFilter","e-");
	primitive = new G4PSNofSecondary("nSecondary");
	absDetector->RegisterPrimitive(primitive);
	primitive = new G4PSNofSecondary("nGamma");
	primitive->SetFilter(gammaFilter);
	absDetector->RegisterPrimitive(primitive); 
	primitive = new G4PSNofSecondary("nElectron");
	primitive->SetFilter(electronFilter);
	absDetector->RegisterPrimitive(primitive); 
	
	// declare Gap as a MultiFunctionalDetector scorer
	//  
	G4MultiFunctionalDetector* gapDetector = new G4MultiFunctionalDetector("Gap");
	
	primitive = new G4PSEnergyDeposit("Edep");
	gapDetector->RegisterPrimitive(primitive);
	
	primitive = new G4PSTrackLength("TrackLength");
	primitive ->SetFilter(charged);
	gapDetector->RegisterPrimitive(primitive);  
	
	G4SDManager::GetSDMpointer()->AddNewDetector(gapDetector);
	gapLV->SetSensitiveDetector(gapDetector);  
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
	
	//
	// Always return the physical World
	//
	return worldPV;
}

