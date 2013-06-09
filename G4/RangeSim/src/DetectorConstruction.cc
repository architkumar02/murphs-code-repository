
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"

#include <iomanip>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  DetectorConstruction::DetectorConstruction()
:pBox(0), lBox(0), aMaterial(0), magField(0)
{
  BoxSize = 10*m;
  DefineMaterials();
  SetMaterial("G4_POLYSTYRENE");  
  detectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete detectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


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
  G4Element* eB = nistManager->FindOrBuildElement("B",fromIsotopes);
  G4Element* eC = nistManager->FindOrBuildElement("C",fromIsotopes);
  G4Element* eO = nistManager->FindOrBuildElement("O",fromIsotopes);
  G4Element* eN = nistManager->FindOrBuildElement("N",fromIsotopes);
  G4Element* eF = nistManager->FindOrBuildElement("F",fromIsotopes);
  G4Element* eS = nistManager->FindOrBuildElement("S",fromIsotopes);
  G4Element* eZn = nistManager->FindOrBuildElement("Zn",fromIsotopes);

  // defining enriched Lithium 6
  G4double a6Li = 6.015*g/mole; // Molar Masses (Wolfram Alpha)
  G4double a7Li = 7.016*g/mole;
  G4double enrichement = 110.815*perCent;
  G4double abundance6Li = enrichement*a6Li/a7Li;                // Relative Abudadance
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
  G4double fractionPPO = 46./(46.+1.36);                // Scintillant is in the ratio of 46 g PPO to 1.36 g POPOP
  G4Material* scintillant = new G4Material("PPO/POPOP",density=1.1*g/cm3,nComponents=2,kStateSolid);
  scintillant->AddMaterial(PPO,fractionPPO);
  scintillant->AddMaterial(POPOP,1-fractionPPO);

  // Polymer PS Based Detector
  G4double fractionPolymer = 0.85;
  G4double fractionScintillant = 0.05;
  G4double fractionAbsorber = 0.10;


  G4Material* psDetector;
  for (int i = 0; i < 3; i++){
    std::ostringstream oss;
    oss << "PS_LiF-"<<std::setprecision(2)<<fractionAbsorber<<"_PPOPOPOP-"<<fractionScintillant;
    psDetector = new G4Material(oss.str(),density=1.1*g/cm3,nComponents=3,kStateSolid);
    psDetector->AddMaterial(nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes),fractionPolymer);
    psDetector->AddMaterial(scintillant,fractionScintillant);
    psDetector->AddMaterial(LiAbsorber,fractionAbsorber);
    fractionAbsorber += 0.10;
  } 
 
  // Defining PEN Based Films
  G4Material* PEN = new G4Material("PEN",density=1.33*g/cm3,nComponents=3);
  PEN->AddElement(eC,nAtoms=14);
  PEN->AddElement(eH,nAtoms=14);
  PEN->AddElement(eO,nAtoms=6);
  
  G4Material* penDetector;
  for (int i = 0; i < 3; i++){
    std::ostringstream oss;
    oss << "PEN_LiF-"<<std::setprecision(2)<<fractionAbsorber<<"_PPOPOPOP-"<<fractionScintillant;
    penDetector = new G4Material(oss.str(),density=1.1*g/cm3,nComponents=3,kStateSolid);
    penDetector->AddMaterial(nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes),fractionPolymer);
    penDetector->AddMaterial(scintillant,fractionScintillant);
    penDetector->AddMaterial(LiAbsorber,fractionAbsorber);
    fractionAbsorber += 0.05;
  } 

  // Defining EJ426 HD2
  G4double massFraction;
  G4Material* EJ426HD2 = new G4Material("EJ426HD2",density=4.1*g/cm3,nComponents=4);
  EJ426HD2->AddElement(enrichLi,massFraction=0.081);
  EJ426HD2->AddElement(eF,massFraction=0.253);
  EJ426HD2->AddElement(eZn,massFraction=0.447);
  EJ426HD2->AddElement(eS,massFraction=0.219);

  // Defining Boron Loaded Plastic
  G4Material* BoronScint;
  G4Material* pvt = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE",fromIsotopes);
  G4Material* carborane = new G4Material("Carborane",density=1.0*g/cm3,nComponents=3,kStateSolid);
  carborane->AddElement(eC,nAtoms=2);
  carborane->AddElement(eB,nAtoms=10);
  carborane->AddElement(eH,nAtoms=12);
  G4double massFracCarborane = 0.00;
  for (int i = 0; i < 6; i++){
    std::ostringstream oss;
    oss << "MS"<<i;
    BoronScint = new G4Material(oss.str(),density=1.1*g/cm3,nComponents=2,kStateSolid);
    BoronScint->AddMaterial(pvt,1.00-massFracCarborane);
    BoronScint->AddMaterial(carborane,massFracCarborane);
    massFracCarborane += 0.01;
  }


  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);

  nistManager->FindOrBuildMaterial("G4_PLEXIGLASS",fromIsotopes);
  nistManager->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes);
  nistManager->FindOrBuildMaterial("G4_AIR",fromIsotopes);
  nistManager->FindOrBuildMaterial("G4_WATER",fromIsotopes);
  nistManager->FindOrBuildMaterial("G4_Al",fromIsotopes); 

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4Box*
    sBox = new G4Box("Container",				//its name
        BoxSize/2,BoxSize/2,BoxSize/2);	//its dimensions

  lBox = new G4LogicalVolume(sBox,			//its shape
      aMaterial,			//its material
      aMaterial->GetName());	//its name

  pBox = new G4PVPlacement(0,				//no rotation
      G4ThreeVector(),		//at (0,0,0)
      lBox,			//its logical volume			   
      aMaterial->GetName(),	//its name
      0,				//its mother  volume
      false,			//no boolean operation
      0);				//copy number

  PrintParameters();

  //always return the root volume
  //
  return pBox;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n The Box is " << G4BestUnit(BoxSize,"Length")
    << " of " << aMaterial->GetName() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  if (pttoMaterial) aMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSize(G4double value)
{
  BoxSize = value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

void DetectorConstruction::SetMagField(G4double fieldValue)
{
  //apply a global uniform magnetic field along Z axis
  G4FieldManager* fieldMgr
    = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if (magField) delete magField;	//delete the existing magn field

  if (fieldValue!=0.)			// create a new one if non nul
  {
    magField = new G4UniformMagField(G4ThreeVector(0.,0.,fieldValue));
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  }
  else
  {
    magField = 0;
    fieldMgr->SetDetectorField(magField);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructVolumes());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
