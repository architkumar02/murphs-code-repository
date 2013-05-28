#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


DetectorMessenger::DetectorMessenger(
                                           DetectorConstruction* Det)
:Detector(Det)
{ 
  PolymerTransportDir = new G4UIdirectory("/PolymerTransport/");
  PolymerTransportDir->SetGuidance("UI commands of this example");
  
  detDir = new G4UIdirectory("/PolymerTransport/material/");
  detDir->SetGuidance("Material Definations");
       
    
  AbsThickCmd = new G4UIcmdWithADoubleAndUnit("/PolymerTransport/det/setAbsThick",this);
  AbsThickCmd->SetGuidance("Set Thickness of the Absorber");
  AbsThickCmd->SetParameterName("Size",false);
  AbsThickCmd->SetRange("Size>=0.");
  AbsThickCmd->SetUnitCategory("Length");
  AbsThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  GapThickCmd = new G4UIcmdWithADoubleAndUnit("/PolymerTransport/det/setGapThick",this);
  GapThickCmd->SetGuidance("Set Thickness of the Gap");
  GapThickCmd->SetParameterName("Size",false);
  GapThickCmd->SetRange("Size>=0.");
  GapThickCmd->SetUnitCategory("Length");  
  GapThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SizeRadiusCmd = new G4UIcmdWithADoubleAndUnit("/PolymerTransport/det/setRadius",this);
  SizeRadiusCmd->SetGuidance("Set tranverse size of the calorimeter");
  SizeRadiusCmd->SetParameterName("Size",false);
  SizeRadiusCmd->SetRange("Size>0.");
  SizeRadiusCmd->SetUnitCategory("Length");    
  SizeRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/PolymerTransport/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
}

DetectorMessenger::~DetectorMessenger()
{
  delete AbsMaterCmd; delete GapMaterCmd;
  delete AbsThickCmd; delete GapThickCmd;
  delete SizeRadiusCmd;   delete UpdateCmd;
  delete detDir;
  delete PolymerTransportDir;  
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == AbsMaterCmd )
   { Detector->SetAbsorberMaterial(newValue);}
   
  if( command == GapMaterCmd )
   { Detector->SetGapMaterial(newValue);}
  
  if( command == AbsThickCmd )
   { Detector->SetAbsorberThickness(AbsThickCmd
                                               ->GetNewDoubleValue(newValue));}
   
  if( command == GapThickCmd )
   { Detector->SetGapThickness(GapThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == SizeRadiusCmd )
   { Detector->SetCaloRadius(SizeRadiusCmd->GetNewDoubleValue(newValue));}
   
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

}

