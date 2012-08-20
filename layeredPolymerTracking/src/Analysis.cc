#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

#include "G4String.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "G4Trajectory.hh"
#include "G4RichTrajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4RichTrajectoryPoint.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

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
            "EventID:ParentID:TrackID:pvNum:layerNum:posX:posY:posZ:kinE:pdgID");

}

void Analysis::EndOfEvent(const G4Event* event){
    // Allocating defaults for items to fill in runTuple
    G4int eventID;
    G4int parentID;
    G4int trackID;
    int copyNum = -1;
    int pVNum = -1;
    G4double posX;
    G4double posY;
    G4double posZ;
    G4double kinE;
    G4int pdgID;

    eventID = event->GetEventID();

    // Getting the trajectories
    G4TrajectoryContainer* trajCont = event->GetTrajectoryContainer();
    G4int nTraj = 0;
    if (trajCont){
        nTraj = trajCont->entries();

        // Looping through all of the trajectories
        for(int i = 0; i < nTraj; i++){
            G4RichTrajectory* traj =(G4RichTrajectory*) (*trajCont)[i];
            parentID = traj->GetParentID();
            trackID = traj->GetTrackID();
            pdgID = traj->GetPDGEncoding();
            kinE = traj->GetInitialKineticEnergy();

            // Looping through the Trajectory Attributes to get the IVPath
            std::vector<G4AttValue> *atts = traj->CreateAttValues();
            std::vector<G4AttValue>::iterator itr;
            for(itr = atts->begin(); itr != atts->end(); itr++){
                if (itr->GetName().compareTo("IVPath")){
                    copyNum = GetCopyNumber(itr->GetValue());
                    pVNum = GetVolumeNumber(itr->GetValue());
                    break;
                }
            }


            // Getting the position of interaction 
            G4RichTrajectoryPoint *p = (G4RichTrajectoryPoint*) traj->GetPoint(0);
            posX = p->GetPosition().getX();
            posY = p->GetPosition().getY();
            posZ = p->GetPosition().getZ();

            // Filling runTuple
            runTuple->Fill(eventID,parentID,trackID,pVNum,copyNum,
                    posX,posY,posZ,kinE,pdgID);
        }
    }


    // Looping through the hit collection
    /*
    G4VHitsCollection *hc = event->GetHCofThisEvent()->GetHC(0);
    for(int i = 0; i < hc->GetSize(); i++){
        // Works 8/19/2012, MJU, 9:40 PM
        //hc->GetHit(i)->Print();
    }
    */
}

/**
 * GetCopyNumber
 *
 * @param G4String s - detector string
 *
 * @brief Parses string s for the layer number.
 * Returns either the layer number, or -1 if not a layer
 */
int Analysis::GetCopyNumber(G4String input){

    std::string s = (std::string) input;
    std::string token;
    std::string pVolume;
    std::string sCopyNum;
    std::istringstream iss(s);
    while ( getline(iss,token,'/')) {
        size_t loc = token.find(":");
        pVolume = token.substr(0,loc);
        sCopyNum = token.substr(loc+1);
        if( pVolume.compare("Layer") ==0){
            return atoi(sCopyNum.c_str());
        }
    }
    return -1;
}
/**
 * GetVolumeNumber
 *
 * @param G4String input
 *
 * @brief Takes an input string an parses it for physical volume
 *
 */
int Analysis::GetVolumeNumber(G4String input){

    std::string s = (std::string) input;
    std::string token;
    std::string pVolume;
    std::istringstream iss(s);
    while ( getline(iss,token,'/')) {
        size_t loc = token.find(":");
        pVolume = token.substr(0,loc);

        // Switching based on the pVolume
        if( pVolume.compare("World") ==0)
            return WORLDNUM;
        else if (pVolume.compare("Absorber") == 0)
            return ABSNUM;
        else if (pVolume.compare("Gap") == 0)
            return GAPNUM;

    }
    return -1;
}

/**
 * EndOfRun
 *
 * Called at the end of a run, which summerizes the run
 */
void Analysis::EndOfRun(const G4Run* aRun){

    outfile->Write();
    outfile->Close();
    delete outfile;
}
