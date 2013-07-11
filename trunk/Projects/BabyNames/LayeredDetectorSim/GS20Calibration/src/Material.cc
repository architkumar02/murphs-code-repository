#include "Materials.hh"

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"

/**
 * Constructor for the Optical Materials
 *
 * The optical materials are implemented as a static class
 */
Materials::Materials(){
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

Materials::~Materials(){
  delete Teflon;      /* Teflon Tape    */
  delete GS20;        /* GS20 Detector  */
  delete BK7;         /* PMT Window Glass (Boroscilate) */
  delete Silicone;    /* Optical Grease */
  delete Air;         /* Air            */
  delete BlackTape;   /* Black tape     */
}

Materials* Materials::Instance(){
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
G4Material* Materials::GetMaterial(const G4String material){
  
  // Trying to build the material
  G4Material* mat = nistMan->FindOrdBuildMaterial(material);
  if (!mat) mat = G4Material::GetMaterial(material);

  // Exceptional error if it isn't found
  if(!mat) {
    std::ostringstream o;
    o<<"Materail "<<material<<" not found.";
    G4Exception("Materials::GetMaterial","",FatalException,o.str(),c_str());
  }
  return mat;
}

/**
 * Creates the optical materials
 */
void Materials::CreateMaterials(){
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
void Materials:: SetOpticalPropertiesTeflon(){

}
/**
 * Sets the optical properties of GS20
 */
void Materials::SetOpticalPropertiesGS20();
/**
 * Sets the optical properties of BK7
 */
void Materials:: SetOpticalPropertiesBK7();
/**
 * Sets the optical properties of Silicone
 *
 * These values are from the GEANT4 WLS Example
 */
void Materials::SetOpticalPropertiesSilicone(){
  const G4int nEntries = 50;

  G4double PhotonEnergy[nEntries] =
  {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};
  G4double AbsClad[nEntries] =
  {20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m};

   G4double RefractiveIndexSilicone[nEntries] =
   { 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46};

  // Add entries into properties table
  G4MaterialPropertiesTable* MPTSilicone = new G4MaterialPropertiesTable();
  MPTSilicone->AddProperty("RINDEX",PhotonEnergy,RefractiveIndexSilicone,nEntries);
  MPTSilicone->AddProperty("ABSLENGTH",PhotonEnergy,AbsClad,nEntries);
  Silicone->SetMaterialPropertiesTable(MPTSilicone);
  
}
/**
 * Sets the optical properties of Air
 *
 * These values are from the GEANT4 WLS example
 */
void Materials:: SetOpticalPropertiesAir(){
  const G4int nEntries = 50;

  G4double PhotonEnergy[nEntries] =
  {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  G4double RefractiveIndex[nEntries] =
  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};

  G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
  MPT->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntries);
  Air->SetMaterialPropertiesTable(MPT);
}
