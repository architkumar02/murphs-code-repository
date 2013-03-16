#include "AnalysisMessenger.hh"

#include "Analysis.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

AnalysisMessenger::AnalysisMessenger(){ 
  /* Command Directory */
  AnalysisDir = new G4UIdirectory("/Analysis/");
  AnalysisDir->SetGuidance("Analysis Commands");
 
  /* Particle Filename */
  ParticleNameCmd = new G4UIcmdWithAString("/Analysis/setParticleName",this);
  ParticleNameCmd->SetGuidance("Set the incident particle name");
  ParticleNameCmd->SetParameterName("choice",false);
  ParticleNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  /* Histogram Boundries */
  HistEMaxCmd = new G4UIcmdWithADoubleAndUnit("/Analysis/setHistogramMax",this);
  HistEMaxCmd->SetGuidance("Set Thickness of the Absorber");
  HistEMaxCmd->SetParameterName("Size",false);
  HistEMaxCmd->SetRange("Size>=0.");
  HistEMaxCmd->SetUnitCategory("Energy");
  HistEMaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
      
}

AnalysisMessenger::~AnalysisMessenger(){
  delete ParticleNameCmd;
  delete HistEMaxCmd; 
  delete AnalysisDir;  
}

void AnalysisMessenger::SetNewValue(G4UIcommand* command,G4String newValue){ 
  if( command == ParticleNameCmd ){
    Analysis::GetInstance()->SetIncidentParticleName(newValue);
  }
   
  if( command == HistEMaxCmd ){
    G4double value = HistEMaxCmd->GetNewDoubleValue(newValue);
    Analysis::GetInstance()->SetHistEMax(value);
  }
}
