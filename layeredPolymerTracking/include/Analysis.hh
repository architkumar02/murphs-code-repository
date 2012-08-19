#ifndef Analysis_h
#define Analysis_h 1

#include "g4analysis_defs.hh"
#include "G4Event.hh"
#include "G4Run.hh"

#include "TNtuple.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TFile.h"

#include "CaloHit.hh"

#include "globals.hh"

// How do we want to save the results?
using namespace G4Root;

#define NUMLAYERS 1

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
    
    // Event NTuple, last bin is sum of all layers
    TNtuple* runTupleGap[NUMLAYERS+1];        // Gap 
    TNtuple* runTupleAbs[NUMLAYERS+1];        // Absorber
    TNtuple* runTupleLayer[NUMLAYERS+1];      // Layer

    // Energy Deposition - using the last array position as the total for 
    // all of the layers
    G4double thisEventTotEGap[NUMLAYERS+1];   // E Dep in each layer of gap
    G4double thisEventTotEAbs[NUMLAYERS+1];   // E Dep in each layer of Abs
    G4int    thisEventTotNumSecondaries[NUMLAYERS+1];

    G4double thisRunTotEGap[NUMLAYERS+1];   // E Dep in each layer of gap
    G4double thisRunTotEAbs[NUMLAYERS+1];   // E Dep in each layer of Abs
    G4int    thisRunTotNumSecondaries[NUMLAYERS+1];

    G4int thisEventSecondaries;
    G4int thisRunTotSecondaries;

    G4int thisEventNumGammas;
    G4int thisRunNumGammas;

    G4int thisEventNumElectrons;
    G4int thisRunNumElectrons;

    TProfile* hProfileEGap;             // Average energy in the Gap, per Layer
    TProfile* hProfileEAbs;             // Average energy in the abs, per layer
    TProfile* hProfileNumSec;           // Average num of secondaries, per layer
    
    
    TH1F* hTotEGap;
    TH1F* hTotEAbs;
    TH1F *hEDepGapLayer[NUMLAYERS];     // Energy dep per layer, histogram
    TH1F *hEDepAbsLayer[NUMLAYERS];     // Energy dep per layer, histogram

    TFile* outfile;
};
#endif
