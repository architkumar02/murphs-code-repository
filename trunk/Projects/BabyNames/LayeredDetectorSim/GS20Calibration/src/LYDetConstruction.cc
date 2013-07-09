#include "DetectorConstruction.h"

#include "Detector.h"
#include "G4Box.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction() {
	defineElements();
	defineMaterials();
	defineOpticalMaterialProperties();
	defineOpticalSurfaces();
}

DetectorConstruction::~DetectorConstruction() {
	// GEANT4 does not delete G4OpticalSurfaces
	delete _reflectiveSurface->GetMaterialPropertiesTable();
	_reflectiveSurface->SetMaterialPropertiesTable(NULL);
	delete _reflectiveSurface;

	// GEANT4 does not delete G4Elements / G4Materials that we defined
	while (_materials.size() > 0) {
		G4Material* material = _materials.back();
		_materials.pop_back();
		if (material) {
			if (material->GetMaterialPropertiesTable()) {
				delete material->GetMaterialPropertiesTable();
				material->SetMaterialPropertiesTable(NULL);
			}
			delete material;
		}
	}

	// delete elements after materials
	while (_elements.size() > 0) {
		G4Element* element = _elements.back();
		_elements.pop_back();
		if (element) {
			delete element;
		}
	}
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
	// we create a lot of objects here. GEANT4 will clean them up for us.

	////////////////////////////////// WORLD //////////////////////////////////
	// first create the shape of the world world by defining a G4VSolid
	G4Box* world = new G4Box("World", 0.8 * CLHEP::m, 0.8 * CLHEP::m, 0.8
			* CLHEP::m);

	// logical volume defines the volume but not its placement in the world
	G4LogicalVolume* logicalWorld = new G4LogicalVolume(world,
			G4Material::GetMaterial("Air", true), "World");

	// physical volume places logical volume somewhere in the world, you can
	// place a logical volume multiple times
	G4VPhysicalVolume* physicalWorld = new G4PVPlacement(0, G4ThreeVector(),
			logicalWorld, "World", 0, false, 0, true);

	/////////////////////////////// SCINTILLATOR //////////////////////////////
	// define scintillator 2cm X 40cm * 1 cm in size
	G4Box* scintillator = new G4Box("Scintillator", 1 * CLHEP::cm, 20
			* CLHEP::cm, 0.5 * CLHEP::cm);

	G4LogicalVolume* logicalScintillator = new G4LogicalVolume(scintillator,
			G4Material::GetMaterial("Polystyrene", true), "Scintillator");

	G4VisAttributes* visScintillator = new G4VisAttributes(true);
	visScintillator->SetColour(0, 0, 1);
	visScintillator->SetLineWidth(2.);
	//visScintillator->SetForceAuxEdgeVisible(true);
	logicalScintillator->SetVisAttributes(visScintillator);

	G4PVPlacement* physicalScintillator = new G4PVPlacement(0, G4ThreeVector(),
			logicalScintillator, "Scintillator", logicalWorld, false, 0, true);

	/////////////////////////////////// PMT ///////////////////////////////////
	// define something shaped like a photomultiplier
	G4Tubs* photomultiplier = new G4Tubs("Photomultiplier", 0, 2.5 * CLHEP::cm,
			10 * CLHEP::cm, 0, CLHEP::twopi);
	G4LogicalVolume* logicalPhotomultiplier = new G4LogicalVolume(
			photomultiplier,
			G4Material::GetMaterial("Borosilicate Glass", true),
			"Photomultiplier");


	G4VisAttributes* visPMT = new G4VisAttributes(true);
	visPMT->SetColour(111./255., 37. / 255., 0);
	visPMT->SetLineWidth(2.);
	visPMT->SetForceSolid(true);
	logicalPhotomultiplier->SetVisAttributes(visPMT);

	// we need to rotate and translate the defined volume in the world to place
	// it with respect to the scintillator
	new G4PVPlacement(new G4RotationMatrix(CLHEP::HepRotationX(CLHEP::halfpi)),
			G4ThreeVector(0, -30 * CLHEP::cm, 0), logicalPhotomultiplier,
			"Photomultiplier", logicalWorld, false, 0, true);

	// set sensitive detector for scintillator
	Detector* detector = new Detector("detector");
	logicalPhotomultiplier->SetSensitiveDetector(detector);

	///////////////////////////// OPTICAL SURFACES ////////////////////////////
	// this will add a reflecting surface to the scintillator where it is not
	// covered by the PMT. We claim a dielectric_metal interface between
	// scintillator and air in order to force reflection
	new G4LogicalBorderSurface("Scintillator-Air", physicalScintillator,
			physicalWorld, _reflectiveSurface);

	// return pointer to the world
	return physicalWorld;
}

void DetectorConstruction::defineElements() {
	// define Elements. They will be stored  and cleaned up by GEANT4
	_elements.push_back(new G4Element("Hydrogen", "H", 1, 1.008 * CLHEP::g
			/ CLHEP::mole));
	_elements.push_back(new G4Element("Boron", "B", 5, 10.811 * CLHEP::g
			/ CLHEP::mole));
	_elements.push_back(new G4Element("Carbon", "C", 6, 12.011 * CLHEP::g
			/ CLHEP::mole));
	_elements.push_back(new G4Element("Nitrogen", "N", 7, 14.0067 * CLHEP::g
			/ CLHEP::mole));
	_elements.push_back(new G4Element("Oxygen", "O", 8, 15.9994 * CLHEP::g
			/ CLHEP::mole));
	_elements.push_back(new G4Element("Sodium", "Na", 11, 22.98977 * CLHEP::g
			/ CLHEP::mole));
	_elements.push_back(new G4Element("Aluminum", "Al", 13, 26.981538
			* CLHEP::g / CLHEP::mole));
	_elements.push_back(new G4Element("Silicon", "Si", 14, 28.0855 * CLHEP::g
			/ CLHEP::mole));
	_elements.push_back(new G4Element("Argon", "Ar", 18, 39.948 * CLHEP::g
			/ CLHEP::mole));
	_elements.push_back(new G4Element("Potassium", "K", 19, 39.0983 * CLHEP::g
			/ CLHEP::mole));
}

void DetectorConstruction::defineMaterials() {
	// define Materials. They will be stored and cleaned up by GEANT4.
	G4Material* air = new G4Material("Air", 1.29 * CLHEP::kg / CLHEP::m3, 3,
			kStateGas);
	air->AddElement(G4Element::GetElement("Nitrogen", true), 78);
	air->AddElement(G4Element::GetElement("Oxygen", true), 21);
	air->AddElement(G4Element::GetElement("Argon", true), 1);
	_materials.push_back(air);

	G4Material* polystyrene = new G4Material("Polystyrene", 1.0320 * CLHEP::g
			/ CLHEP::cm3, 2, kStateSolid);
	polystyrene->AddElement(G4Element::GetElement("Hydrogen", true), 8);
	polystyrene->AddElement(G4Element::GetElement("Carbon", true), 8);
	_materials.push_back(polystyrene);

	G4Material* borosilicateGlass = new G4Material("Borosilicate Glass", 2.23
			* CLHEP::g / CLHEP::cm3, 6, kStateSolid);
	borosilicateGlass->AddElement(G4Element::GetElement("Boron", true),
			0.040064);
	borosilicateGlass->AddElement(G4Element::GetElement("Oxygen", true),
			0.539562);
	borosilicateGlass->AddElement(G4Element::GetElement("Sodium", true),
			0.028191);
	borosilicateGlass->AddElement(G4Element::GetElement("Aluminum", true),
			0.011644);
	borosilicateGlass->AddElement(G4Element::GetElement("Silicon", true),
			0.377220);
	borosilicateGlass->AddElement(G4Element::GetElement("Potassium", true),
			0.003321);
	_materials.push_back(borosilicateGlass);
}

void DetectorConstruction::defineOpticalMaterialProperties() {
	defineOpticalPolystyrene();
	defineOpticalAir();
	defineOpticalBorosilicateGlass();
}

void DetectorConstruction::defineOpticalBorosilicateGlass() {
	// set optical properties of Air
	G4MaterialPropertiesTable* propertiesTable =
			new G4MaterialPropertiesTable();

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

	G4Material::GetMaterial("Borosilicate Glass", true)->SetMaterialPropertiesTable(
			propertiesTable);
}

void DetectorConstruction::defineOpticalAir() {
	// set optical properties of Air
	G4MaterialPropertiesTable* propertiesTable =
			new G4MaterialPropertiesTable();

	// set absorption length to a low value to make display simpler
	G4MaterialPropertyVector* absorption = new G4MaterialPropertyVector();
	// note: GEANT4 may crash when encountering photons with energies outside
	// the boundary energies defined for MaterialPropertiesVectors. Fortunately
	// we control which optical photons are produced!
	absorption->AddElement(0.1 * CLHEP::eV, 1 * CLHEP::mm);
	absorption->AddElement(10. * CLHEP::eV, 1 * CLHEP::mm);
	propertiesTable->AddProperty("ABSLENGTH", absorption);

	// set refractive index
	G4MaterialPropertyVector* rindex = new G4MaterialPropertyVector();
	rindex->AddElement(0.1 * CLHEP::eV, 1.59);
	rindex->AddElement(10. * CLHEP::eV, 1.59);
	propertiesTable->AddProperty("RINDEX", rindex);

	G4Material::GetMaterial("Air", true)->SetMaterialPropertiesTable(
			propertiesTable);
}

void DetectorConstruction::defineOpticalPolystyrene() {
	// set optical properties of Polystyrene
	G4MaterialPropertiesTable* propertiesTable =
			new G4MaterialPropertiesTable();

	// set absorption length
	G4MaterialPropertyVector* absorption = new G4MaterialPropertyVector();
	// note: GEANT4 may crash when encountering photons with energies outside
	// the boundary energies defined for MaterialPropertiesVectors. Fortunately
	// we control which optical photons are produced!
	absorption->AddElement(0.1 * CLHEP::eV, 5 * CLHEP::m);
	absorption->AddElement(10. * CLHEP::eV, 5 * CLHEP::m);
	propertiesTable->AddProperty("ABSLENGTH", absorption);

	// set rayleigh scattering
	G4MaterialPropertyVector* rayleigh = new G4MaterialPropertyVector();
	rayleigh->AddElement(0.1 * CLHEP::eV, 1 * CLHEP::m);
	rayleigh->AddElement(10. * CLHEP::eV, 1 * CLHEP::m);
	propertiesTable->AddProperty("RAYLEIGH", rayleigh);

	// set refractive index
	G4MaterialPropertyVector* rindex = new G4MaterialPropertyVector();
	rindex->AddElement(0.1 * CLHEP::eV, 1.59);
	rindex->AddElement(10. * CLHEP::eV, 1.59);
	propertiesTable->AddProperty("RINDEX", rindex);

	// set the fast component of the scintillation light. GEANT4 could handle
	// two scintillation components, but we don't care for that right now
	G4MaterialPropertyVector* fastComponent = new G4MaterialPropertyVector();
	fastComponent->AddElement(2. * CLHEP::eV, 0.);
	fastComponent->AddElement(2.5 * CLHEP::eV, 1.);
	fastComponent->AddElement(2.7 * CLHEP::eV, 0.);
	propertiesTable->AddProperty("FASTCOMPONENT", fastComponent);

	// set decay constant of scintillation light
	propertiesTable->AddConstProperty("FASTTIMECONSTANT", 2.7 * CLHEP::ns);

	// set number of produced photons per unit of energy deposited in material
	propertiesTable->AddConstProperty("SCINTILLATIONYIELD", 0.2 * 8
			/ CLHEP::keV);

	// yield ratio and resolution scale are required by code
	propertiesTable->AddConstProperty("YIELDRATIO", 1.0);
	propertiesTable->AddConstProperty("RESOLUTIONSCALE", 1.0);

	G4Material::GetMaterial("Polystyrene", true)->SetMaterialPropertiesTable(
			propertiesTable);
}

void DetectorConstruction::defineOpticalSurfaces() {
	_reflectiveSurface = new G4OpticalSurface("Scintillator-Air", glisur,
			ground, dielectric_metal);
	_reflectiveSurface->SetPolish(0.95);

	// here reflectivity of the surface is set
	G4MaterialPropertiesTable* propertiesTable =
			new G4MaterialPropertiesTable();
	G4MaterialPropertyVector* reflectivity = new G4MaterialPropertyVector();
	// reflectivity 0.97
	reflectivity->AddElement(0.1 * CLHEP::eV, 0.92);
	reflectivity->AddElement(10. * CLHEP::eV, 0.92);

	propertiesTable->AddProperty("REFLECTIVITY", reflectivity);

	_reflectiveSurface->SetMaterialPropertiesTable(propertiesTable);
}

