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

#include "TString.h"

Analysis* Analysis::singleton = 0;

Analysis::Analysis(){
    // Empty Constructor, assingment done in constuctor list
}

void Analysis::PrepareNewEvent(const G4Event* anEvent){
    // Nothing to be done before an event
}

void Analysis::PrepareNewRun(const G4Run* aRun){

    char name[256];
    char title[256];
    sprintf(name,"run_%d.root",aRun->GetRunID());
    outfile = new TFile(name,"recreate");

    // Creating NTuples
    runTuple = new TNtuple("runTuple","Run Records",
            "EventID:ParentID:TrackID:pvNum:layerNum:posX:posY:posZ:kinE:pdgID");   
    hitTuple = new TNtuple("hitTuple","Hit Records",
          "xPos:yPos:zPos:layerNum:pvNum:eDep:pdgID:trackID:parentID:eventID");

    // Creating Energy Distributions of Hits (over event) and per Hit
    G4double binMin = 0*eV;
    G4double binMax = 5*MeV;
    G4int numBins = 500;
    for(int i = 0; i < NUMLAYERS; i++){
        sprintf(name,"eventTupleLayer%2i",i);
        sprintf(title,"Total Energy Deposited in an Event (Layer %2i)",i);
        tEventTotEDep[i] = new TNtuple(name,title,
            "eDepGap:eDepAbs:eDepTot");

        sprintf(name,"totHitEDepAbs_%2i",i);
        sprintf(title,"Total Energy Deposited in a Hit (Abs Layer %2i)",i);
        hHitTotEDepAbs[i] = new TH1F(name,title,numBins,binMin,binMax);

        sprintf(name,"totHitEDepGap_%2i",i);
        sprintf(title,"Total Energy Deposited in a Hit (Gap Layer %2i)",i);
        hHitTotEDepGap[i] = new TH1F(name,title,numBins,binMin,binMax);

    }
    tEventTotEDep[NUMLAYERS] = new TNtuple("eventTupleAllLayers",
            "Total Energy Deposited in an Event (all layers)",
            "eDepGap:eDepAbs:eDepTot");

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
            G4RichTrajectoryPoint *p = 
                (G4RichTrajectoryPoint*) traj->GetPoint(0);
            posX = p->GetPosition().getX();
            posY = p->GetPosition().getY();
            posZ = p->GetPosition().getZ();

            // Filling runTuple
            runTuple->Fill(eventID,parentID,trackID,pVNum,copyNum,
                    posX,posY,posZ,kinE,pdgID);
        }
    }

    // Processing all of the hit collections
    G4int numHitColl = event->GetHCofThisEvent()->GetNumberOfCollections();
    for(G4int i = 0; i < numHitColl; i++){
        ProcessHitCollection( event->GetHCofThisEvent()->GetHC(i),eventID);
    }
}

/**
 * ProcessHitCollection
 *
 * @param G4VHitsCollection *hc
 *
 * Helper method to process hit collections
 */
void Analysis::ProcessHitCollection(G4VHitsCollection *hc,G4int eventID){

    // Looping through the hit collection
    G4double eventEDepAbs[NUMLAYERS+1];
    G4double eventEDepGap[NUMLAYERS+1];
    for(int i= 0; i < NUMLAYERS+1; i++){
        eventEDepAbs[i] = 0.0;
        eventEDepGap[i] = 0.0;
    }

    // Energy Deposition of the event
    for(int i = 0; i < hc->GetSize(); i++){
        CaloHit* hit = (CaloHit*) hc->GetHit(i);

        G4double eDep = hit->GetEdep();
        G4int layerNum = hit->GetLayerNumber();
        G4double xPos = hit->GetPosition().getX();
        G4double yPos = hit->GetPosition().getY();
        G4double zPos = hit->GetPosition().getZ();
        G4int pdgID = hit->GetParticle()->GetPDGEncoding();
        G4int trackID = hit->GetTrackID();
        G4int parentID = hit->GetParentID();

        if (strcmp(hit->GetVolume()->GetName(),"Gap")){
            // Hit occured in the Gap
            eventEDepGap[layerNum] += eDep;
            (hHitTotEDepGap[layerNum])->Fill(eDep);
            hitTuple->Fill(xPos,yPos,zPos,layerNum,GAPNUM,eDep,pdgID,
                trackID,parentID,eventID);
        }else if(strcmp(hit->GetVolume()->GetName(),"Absorber")){
            // Hit occured in the Abs
            eventEDepAbs[layerNum] += eDep;
            (hHitTotEDepAbs[layerNum])->Fill(eDep);
            hitTuple->Fill(xPos,yPos,zPos,layerNum,ABSNUM,eDep,pdgID,
                trackID,parentID,eventID);
        }
        else{
            G4cout<<"ERROR - Unkown Volume for sensitive detector"<<G4endl;
        }


        for (int i = 0; i < NUMLAYERS; i++){
            eventEDepAbs[NUMLAYERS] += eventEDepAbs[i];
            eventEDepGap[NUMLAYERS] += eventEDepGap[i];
        }
        // Filling event energy deposition tuple
        for(int i= 0; i < NUMLAYERS+1; i++){
            (tEventTotEDep[i])->Fill(eventEDepAbs[i],eventEDepGap[i],
                    (eventEDepAbs[i]+eventEDepGap[i]));
        }
    }
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
