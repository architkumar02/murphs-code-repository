#include "OpticalMaterials.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"

/**
 * Constructor for the Optical Materials
 *
 * The optical materials are implemented as a static class
 */
OpticalMaterials::OpticalMaterials(){
  // Getting the NIST Material Manager
  nistMan = G4NistManager::Instance();
  nistMan->SetVerbose(2);

  // Creating my custom materials
  CreateMaterials();
  SetOpticalPropertiesTeflon();
  SetOpticalPropertiesGS20();
  SetOpticalPropertiesBK7();
  SetOpticalPropertiesSilicone();
  SetOpticalPropertiesAir();

}

OpticalMaterials::~OpticalMaterials(){
  delete Teflon;      /* Teflon Tape    */
  delete GS20;        /* GS20 Detector  */
  delete BK7;         /* PMT Window Glass (Boroscilate) */
  delete Silicone;    /* Optical Grease */
  delete Air;         /* Air            */
  delete BlackTape;   /* Black tape     */
}

OpticalMaterials* OpticalMaterials::Instance(){
  if (instance ==0)
    instance = new OpticalMaterails();
  return instance;
}

/**
 * Searches for the material in the material manager. If the material is not
 * found in the material manager or can not be built, and exception is issued.
 * These are optical materials, implying that they have optical properities
 * ascoated with the material.
 *
 * @param material Material Name
 * @return A pointer to the material
 */
G4Material* OpticalMaterials::GetMaterial(const G4String material){
  
  // Trying to build the material
  G4Material* mat = nistMan->FindOrdBuildMaterial(material);
  if (!mat) mat = G4Material::GetMaterial(material);

  // Exceptional error if it isn't found
  if(!mat) {
    std::ostringstream o;
    o<<"Materail "<<material<<" not found.";
    G4Exception("OpticalMaterials::GetMaterial","",FatalException,o.str(),c_str());
  }
  return mat;
}

/**
 * Creates the optical materials
 */
void OpticalMaterials::CreateMaterials(){
    G4double density;
    G4int ncomponents;
    G4double fractionmass;
    std::vector<G4int> natoms;
    std::vecotor<G4double> fractionMass;
    std::vector<G4String> elements;

    // Material Definations
    
    //-----------------------------------------------------------------------
    // Teflon Tape
    //-----------------------------------------------------------------------
    Teflon = nistMan->FindOrBuildMaterial("G4_TEFLON");
    
    
    //-----------------------------------------------------------------------
    // GS20
    // GS20 is (by mass fraction):
    //  56% SiO2, 4% MgO,  18% Al2O3 18% Li2O, and 4% Ce2O3
    //-----------------------------------------------------------------------
    G4Isotope* Li6 = new G4Isotope(name="Li6",3,6,6.015*g/mole);
    G4Isotope* Li7 = new G4Isotope(name="Li7",3,7,7.015*g/mole);
    G4Element* enrichLi  = new G4Element("enriched Lithium","Li",2);
    enrichLi->AddIsotope(Li6,0.95*perCent);
    enrichLi->AddIsotope(Li7,0.05*perCent);

    G4Material* LiOxide = new G4Material("6LiO2",density=2.013*g/cm3,2);
    LiAbsorber->AddElement(enrichLi,2);
    LiAbsorber->AddElement(nistManager->FindOrBuildElement("O"),1);

    G4Material* SiO2 = nistMan->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    G4Material* MgO = nistMan->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
    G4Material* Al2O3 = nistMan->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
    
    elmenents.push_back("Ce");   natoms.push_back(2);
    elmenents.push_back("O");    natoms.push_back(3);
    density = 6.2*g/cm3;
    G4Material* Ce2O3 = nistMan->ConstructNewMaterial("Ce2O3",elements,natoms,density);
    elmenents.clear();           natoms.clear();

    density = 2.5*g/cm3;
    G4Material* GS20 = new G4Material("GS20",density,5,kStateSolid);
    GS20->AddMaterial(SiO2,56*perCent);
    GS20->AddMaterial(MgO,4*perCent);
    GS20->AddMaterial(Al2O3,18*perCent);
    GS20->AddMaterial(LiOxide,18*perCent);
    GS20->AddMaterial(Ce2O3,4*perCent);

    
    //-----------------------------------------------------------------------
    // BK7 Glass (Boroscilicate glass) 
    // Compositon from www.zcq-quartz.com/gass-2.html
    //
    // SiO2=69.13% B2O3=10.75% BaO=3.07% Na2O=10.40% K2O=6.29% As2O3=0.36%
    //-----------------------------------------------------------------------
    elmenents.push_back("Ba");   natoms.push_back(1);
    elmenents.push_back("O");    natoms.push_back(1);
    density = 5.27*g/cm3;
    G4Material* BaO = nistMan->ConstructNewMaterial("BaO",elements,natoms,density);
    elmenents.clear();           natoms.clear();
    
    G4Material* B2O3 = nistMan->FindOrBuildMaterial("G4_BORON_OXIDE");
    G4Material* Na2O = nistMan->FindOrBuildMaterial("G4_SODIUM_MONOXIDE");
    G4Material* K2O = nistMan->FindOrBuildMaterial("G4_POTASSIUM_OXIDE");
    
    elmenents.push_back("As");   natoms.push_back(2);
    elmenents.push_back("O");    natoms.push_back(3);
    density = 3.74*g/cm3;
    G4Material* BaO = nistMan->ConstructNewMaterial("A2O3",elements,natoms,density);
    elmenents.clear();           natoms.clear();

    G4Material* BK7 = new G4Material("BK7", 2.23*g/cm3,6, kStateSolid);
    BK7->AddMaterial(SiO2,69.13*perCent);
    BK7->AddMaterial(B2O3,10.75*perCent);
    BK7->AddMaterial(BaO,3.07*perCent);
    BK7->AddMaterial(Na2O,10.40*perCent);
    BK7->AddMaterial(K2O,6.29*perCent);
    BK7->AddMaterial(As2O3,0.36*perCent);
    
    //-----------------------------------------------------------------------
    // AIR
    //-----------------------------------------------------------------------
    Air = nistMan->FindOrBuildMaterial("G4_AIR");
    
    //-----------------------------------------------------------------------
    // Silicone Optical Grease
    // Composition from $G4EXAMPLES/extended/wls/src/WLSMaterials.cc
    //-----------------------------------------------------------------------
    elmenents.push_back("C");   natoms.push_back(2);
    elmenents.push_back("H");    natoms.push_back(6);
    density = 1.060*g/cm3;
    Silicone = nistMan->ConstructNewMaterial("Silicone",elements,natoms,density);
    elmenents.clear();           natoms.clear();
  
    
    nistMan->FindOrBuildMaterial("G4_PLEXIGLASS",fromIsotopes);
    nistMan->FindOrBuildMaterial("G4_POLYSTYRENE",fromIsotopes);
    nistMan->FindOrBuildMaterial("G4_Galactic",fromIsotopes);
}

/**
 * Sets the optical properties of Teflon
 */
void OpticalMaterials:: SetOpticalPropertiesTeflon(){

}
/**
 * Sets the optical properties of GS20
 */
void OpticalMaterials::SetOpticalPropertiesGS20();
/**
 * Sets the optical properties of BK7
 */
void OpticalMaterials:: SetOpticalPropertiesBK7();
/**
 * Sets the optical properties of Silicone
 */
void OpticalMaterials::SetOpticalPropertiesSilicone();
/**
 * Sets the optical properties of Air
 */
void OpticalMaterials:: SetOpticalPropertiesAir();

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

