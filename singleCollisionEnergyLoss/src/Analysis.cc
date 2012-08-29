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
#include "G4Event.hh"
#include "G4ThreeVector.hh"

#include "TString.h"

Analysis* Analysis::singleton = 0;

Analysis::Analysis(){
    // Empty Constructor, assingment done in constuctor list
}

void Analysis::PrepareNewEvent(const G4Event* anEvent){

    // Resetting Accumulation Varaibles
}

void Analysis::PrepareNewRun(const G4Run* aRun){

    char name[256];
    sprintf(name,"run_%d.root",aRun->GetRunID());
    outfile = new TFile(name,"recreate");

    // Creating NTuples
    trajTuple = new TNtuple("trajTuple","Run Records",
            "EventID:kinE:fKinE:kinEdE:pdgID");   
}

void Analysis::EndOfEvent(const G4Event* event){
    // Allocating defaults for items to fill in trajTuple
    G4int eventID;
    G4double kinE;
    G4double fKinE;
    G4int pdgID;

    eventID = event->GetEventID();

    // Getting the trajectories
    G4TrajectoryContainer* trajCont = event->GetTrajectoryContainer();
    if (trajCont){

        // Only need to get loop throught the first trajectory, all others are killed
        G4RichTrajectory* traj =(G4RichTrajectory*) (*trajCont)[0];
        pdgID = traj->GetPDGEncoding();
        kinE = traj->GetInitialKineticEnergy();

        // Looping through the Trajectory Attributes to get the IVPath
        std::vector<G4AttValue> *atts = traj->CreateAttValues();
        std::vector<G4AttValue>::iterator itr;
        for(itr = atts->begin(); itr != atts->end(); itr++){
            if (itr->GetName().compareTo("FKE") == 0){
                // Getting Final Kinetic Energy
                float val;
                sscanf(itr->GetValue().data(),"%f eV",&val);
                fKinE = (G4double) val*eV;
                break;
            }
        }
        // Getting the points of interaction along the trajectory 
        /*
        G4RichTrajectoryPoint *p;
        for (G4int j = 0; j < traj->GetPointEntries(); j++){
            p = (G4RichTrajectoryPoint*) traj->GetPoint(j);
            atts = p->CreateAttValues();
            for(itr = atts->begin(); itr != atts->end(); itr++){
            //  G4cout<<itr->GetName()<<"\t"<<itr->GetValue()<<G4endl;
              if (itr->GetName().compareTo("RE") == 0){
                    // Getting  Kinetic Energy
                    float val;
                    sscanf(itr->GetValue().data(),"%f eV",&val);
                    fKinE = (G4double) val;
                }
            }
        }
        */
        // Filling trajTuple
        trajTuple->Fill(eventID,kinE,fKinE,(kinE-fKinE),pdgID);
        /*
        G4cout<<"\n\t Initial Kinetic Energy "<<kinE / eV <<" [eV]"
              <<" Final Kinetic Energy "<<fKinE / eV<<" [eV]"
              <<" Difference "<<(kinE-fKinE) / eV <<" [eV]"<<G4endl;
        */
    }
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
