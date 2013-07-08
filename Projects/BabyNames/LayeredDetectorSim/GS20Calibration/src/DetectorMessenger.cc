/// \file optical//src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
//
#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4Scintillation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
 : fDetector(detector)
{
  //Setup a command directory for detector controls with guidance
  fDetectorDir = new G4UIdirectory("//detector/");
  fDetectorDir->SetGuidance("Detector geometry control");

  fVolumesDir = new G4UIdirectory("//detector/volumes/");
  fVolumesDir->SetGuidance("Enable/disable volumes");
 
  //Various commands for modifying detector geometry
  fDimensionsCmd =
    new G4UIcmdWith3VectorAndUnit("//detector/dimensions",this);
  fDimensionsCmd->SetGuidance("Set the dimensions of the detector volume.");
  fDimensionsCmd->SetParameterName("scint_x","scint_y","scint_z",false);
  fDimensionsCmd->SetDefaultUnit("cm");

  fHousingThicknessCmd = new G4UIcmdWithADoubleAndUnit
    ("//detector/housingThickness",this);
  fHousingThicknessCmd->SetGuidance("Set the thickness of the housing.");
  fHousingThicknessCmd->SetParameterName("d_mtl",false);
  fHousingThicknessCmd->SetDefaultUnit("cm");

  fPmtRadiusCmd = new G4UIcmdWithADoubleAndUnit
    ("//detector/pmtRadius",this);
  fPmtRadiusCmd->SetGuidance("Set the radius of the PMTs.");
  fPmtRadiusCmd->SetParameterName("radius",false);
  fPmtRadiusCmd->SetDefaultUnit("cm");

  fNxCmd = new G4UIcmdWithAnInteger("//detector/nx",this);
  fNxCmd->SetGuidance("Set the number of PMTs along the x-dimension.");
  fNxCmd->SetParameterName("nx",false);

  fNyCmd = new G4UIcmdWithAnInteger("//detector/ny",this);
  fNyCmd->SetGuidance("Set the number of PMTs along the y-dimension.");
  fNyCmd->SetParameterName("ny",false);

  fNzCmd = new G4UIcmdWithAnInteger("//detector/nz",this);
  fNzCmd->SetGuidance("Set the number of PMTs along the z-dimension.");
  fNzCmd->SetParameterName("nz",false);

  fSphereCmd = new G4UIcmdWithABool("//detector/volumes/sphere",this);
  fSphereCmd->SetGuidance("Enable/Disable the sphere.");

  fReflectivityCmd = new G4UIcmdWithADouble("//detector/reflectivity",this);
  fReflectivityCmd->SetGuidance("Set the reflectivity of the housing.");

  fWlsCmd = new G4UIcmdWithABool("//detector/volumes/wls",this);
  fWlsCmd->SetGuidance("Enable/Disable the WLS slab");

  fLxeCmd = new G4UIcmdWithABool("//detector/volumes/lxe",this);
  fLxeCmd->SetGuidance("Enable/Disable the main detector volume.");

  fNFibersCmd = new G4UIcmdWithAnInteger("//detector/nfibers",this);
  fNFibersCmd->SetGuidance("Set the number of WLS fibers in the WLS slab.");

  fUpdateCmd = new G4UIcommand("//detector/update",this);
  fUpdateCmd->SetGuidance("Update the detector geometry with changed values.");
  fUpdateCmd->SetGuidance
    ("Must be run before beamOn if detector has been changed.");

  fDefaultsCmd = new G4UIcommand("//detector/defaults",this);
  fDefaultsCmd->SetGuidance("Set all detector geometry values to defaults.");
  fDefaultsCmd->SetGuidance("(Update still required)");

  fMainScintYield=new G4UIcmdWithADouble("//detector/MainScintYield",this);
  fMainScintYield->SetGuidance("Set scinitillation yield of main volume.");
  fMainScintYield->SetGuidance("Specified in photons/MeV");

  fWLSScintYield = new G4UIcmdWithADouble("//detector/WLSScintYield",this);
  fWLSScintYield->SetGuidance("Set scintillation yield of WLS Slab");
  fWLSScintYield->SetGuidance("Specified in photons/MeV");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fDimensionsCmd;
  delete fHousingThicknessCmd;
  delete fPmtRadiusCmd;
  delete fNxCmd;
  delete fNyCmd;
  delete fNzCmd;
  delete fUpdateCmd;
  delete fDetectorDir;
  delete fVolumesDir;
  delete fDefaultsCmd;
  delete fSphereCmd;
  delete fWlsCmd;
  delete fLxeCmd;
  delete fNFibersCmd;
  delete fReflectivityCmd;
  delete fMainScintYield;
  delete fWLSScintYield;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == fDimensionsCmd ){
    fDetector->SetDimensions(fDimensionsCmd->GetNew3VectorValue(newValue));
  }
  else if (command == fHousingThicknessCmd){
    fDetector->SetHousingThickness(fHousingThicknessCmd
                                     ->GetNewDoubleValue(newValue));
  }
  else if (command == fPmtRadiusCmd){
    fDetector->SetPMTRadius(fPmtRadiusCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fNxCmd){
    fDetector->SetNX(fNxCmd->GetNewIntValue(newValue));
  }
  else if (command == fNyCmd){
    fDetector->SetNY(fNyCmd->GetNewIntValue(newValue));
  }
  else if (command == fNzCmd){
    fDetector->SetNZ(fNzCmd->GetNewIntValue(newValue));
  }
  else if (command == fUpdateCmd){
    fDetector->UpdateGeometry();
  }
  else if (command == fDefaultsCmd){
    fDetector->SetDefaults();
  }
  else if (command == fSphereCmd){
    fDetector->SetSphereOn(fSphereCmd->GetNewBoolValue(newValue));
  }
  else if (command == fReflectivityCmd){
    fDetector
      ->SetHousingReflectivity(fReflectivityCmd->GetNewDoubleValue(newValue));
  }
  else if (command == fWlsCmd){
    fDetector->SetWLSSlabOn(fWlsCmd->GetNewBoolValue(newValue));
  }
  else if (command == fLxeCmd){
    fDetector->SetMainVolumeOn(fLxeCmd->GetNewBoolValue(newValue));
  }
  else if (command == fNFibersCmd){
    fDetector->SetNFibers(fNFibersCmd->GetNewIntValue(newValue));
  }
  else if (command == fMainScintYield){
   fDetector->SetMainScintYield(fMainScintYield->GetNewDoubleValue(newValue));
  }
  else if (command == fWLSScintYield){
    fDetector->SetWLSScintYield(fWLSScintYield->GetNewDoubleValue(newValue));
  }
}
