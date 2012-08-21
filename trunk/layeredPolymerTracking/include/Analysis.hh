#ifndef Analysis_h
#define Analysis_h 1

#include "g4analysis_defs.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"

#include "TNtuple.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TFile.h"

#include "CaloHit.hh"

#include "G4String.hh"
#include "globals.hh"

// How do we want to save the results?
using namespace G4Root;

#define NUMLAYERS 1     // Number of layers in the detector
#define WORLDNUM 0      // Map between material and Number
#define GAPNUM 1
#define ABSNUM 2

class Analysis {

public:
    // Singleton
    static Analysis* GetInstance(){
       if(Analysis::singleton == NULL)
           Analysis::singleton = new Analysis();
       return Analysis::singleton;
    }

    virtual ~Analysis() {};

    void PrepareNewEvent(const G4Event* anEvent);
    void PrepareNewRun(const G4Run* aRun);
    void EndOfEvent(const G4Event* anEvent);
    void EndOfRun(const G4Run* aRun);

private:
    // Singleton Analysis
    Analysis();
    static Analysis *singleton;

    void ProcessHitCollection(G4VHitsCollection* hc, G4int eventID);
    int GetCopyNumber(G4String s);
    int GetVolumeNumber(G4String s);
    
    // Run NTuple
    TNtuple* runTuple; 

    // Energy Deposition - using the last array position as the total for 
    // all of the layers
    TNtuple* tEventTotEDep[NUMLAYERS+1];
    TNtuple* hitTuple;
    TH1F* hHitTotEDepGap[NUMLAYERS]; 
    TH1F* hHitTotEDepAbs[NUMLAYERS]; 

    TFile* outfile;
};
#endif
