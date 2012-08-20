#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include <string.h>

#include "G4String.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include <vector>
#include <map>

#include "G4Trajectory.hh"
#include "G4RichTrajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4RichTrajectoryPoint.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"

Analysis* Analysis::singleton = 0;

Analysis::Analysis(){
    // Empty Constructor, assingment done in constuctor list
}
/*
void Analysis::AddHit(CaloHit* hit){
    G4int layerNum = hit->GetLayerNumber();
    if (strcmp(hit->GetVolume()->GetName(),"Gap")){
        thisEventTotEGap[layerNum] += hit->GetEdep();
    }else if(strcmp(hit->GetVolume()->GetName(),"Absorber")){
        thisEventTotEAbs[layerNum] += hit->GetEdep();
    }
    else{
        G4cout<<"ERROR - Unkown Volume for sensitive detector"<<G4endl;
    }
    if (hit->GetParticleRank() > 1)
        thisEventTotNumSecondaries[layerNum]++;
}
*/

void Analysis::PrepareNewEvent(const G4Event* anEvent){
    // Nothing to be done before an event
    thisEventSecondaries = 0;
    thisEventNumGammas = 0;
    thisEventNumElectrons = 0;
    for(int i = 0; i<NUMLAYERS+1; i++){
        thisEventTotEGap[i] = 0;
        thisEventTotEAbs[i] = 0;
        thisEventTotNumSecondaries[i] = 0;
    }
}

void Analysis::PrepareNewRun(const G4Run* aRun){

    char filename[256];
    sprintf(filename,"run_%d.root",aRun->GetRunID());
    outfile = new TFile(filename,"recreate");


    // Creating NTuples
    runTuple = new TNtuple("runTuple","Run Records",
        "EventID:ParentID:TrackID:layerNum:IsAbs:pdgID:kinE");

}

void Analysis::EndOfEvent(const G4Event* event){
    // Getting the trajectories
    G4TrajectoryContainer* trajCont = event->GetTrajectoryContainer();
    G4int nTraj = 0;
    if (trajCont){
        nTraj = trajCont->entries();
        G4cout<<"There are "<<nTraj<<" trajectories in event "
              <<event->GetEventID()<<"."<<G4endl;

        // Looping through all of the trajectories
       G4cout<<"\tParticle name\tIntial Kinetic Energy (keV)\tTrackID\tParentID"<<G4endl;
        for(int i = 0; i < nTraj; i++){
            G4RichTrajectory* traj =(G4RichTrajectory*) (*trajCont)[i];
            G4cout<<"\t "<<traj->GetParticleName()
                <<"\t\t "<<traj->GetInitialKineticEnergy()
                <<"\t "<<traj->GetTrackID()
                <<"\t "<<traj->GetParentID()
                <<G4endl;
        
            std::vector<G4AttValue> *atts = traj->CreateAttValues();
            std::vector<G4AttValue>::iterator itr;
            for(itr = atts->begin(); itr != atts->end(); itr++){
                G4cout<<itr->GetName()<<"  "<<itr->GetValue()<<G4endl;
            }

            // Can Get all of the positions and volumes by itterating through
            // G4RichTrajectoryPoint
            
            /*
            G4RichTrajectoryPoint *p = (G4RichTrajectoryPoint*) traj->GetPoint(0);
            G4cout<<p->GetPosition()<<G4endl;
            std::vector<G4AttValue> *atts = p->CreateAttValues();
            std::vector<G4AttValue>::iterator itr;
            for(itr = atts->begin(); itr != atts->end(); itr++){
                G4cout<<itr->GetName()<<"  "<<itr->GetValue()<<G4endl;
            }
            */     
        
        //"EventID:ParentID:TrackID:layerNum:IsAbs:pdgID:kinE");
        }
    }


    // Looping through the hit collection
    G4VHitsCollection *hc = event->GetHCofThisEvent()->GetHC(0);
    for(int i = 0; i < hc->GetSize(); i++){
        // Works 8/19/2012, MJU, 9:40 PM
        //hc->GetHit(i)->Print();
    }
}

/**
 * GetLayerNumber
 *
 * @param G4String s - detector string
 *
 * @brief Parses string s for the layer number.
 * Returns either the layer number, or -1 if not a layer
 */
int Analysis::GetLayerNumber(G4String s){
   
   char *tok = strtok(s,":");
   while (tok != NULL){
        
        if(strcmp(tok,"Layer") ==0){
            tok = strtok(NULL,":");
            return tok[1];
        }
        tok = strtok(NULL,":");
   }
   return -1;
}
/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){
   
    G4cout<<"Testing GetLayerNumber"<<G4endl;
    outfile->Write();
    outfile->Close();
    delete outfile;
}
