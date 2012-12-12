#ifndef Analysis_h
#define Analysis_h 1

#include "g4analysis_defs.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"
#include "DetectorConstruction.hh"

#include "TNtuple.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TFile.h"

#include "CaloHit.hh"

#include "G4String.hh"
#include "globals.hh"

// How do we want to save the results?
using namespace G4Root;

#define WORLDNUM 0      // Map between material and Number
#define GAPNUM 1
#define ABSNUM 2
#define NUMPID 5        // PID of 0, 1, 2,3 (placeholder,neutron,Triton,alpha)
class Analysis {

public:
    Analysis(DetectorConstruction *det);
    
    // Singleton
    static Analysis* GetInstance(){
        /*
       if(Analysis::singleton == NULL)
           Analysis::singleton = new Analysis(DetectorConstruction *det);
      */
      return Analysis::singleton;
    }

    virtual ~Analysis() {};

    // Accumulation Methods
    void PrepareNewEvent(const G4Event* anEvent);
    void PrepareNewRun(const G4Run* aRun);
    void EndOfEvent(const G4Event* anEvent);
    void EndOfRun(const G4Run* aRun);

private:
    // Singleton Analysis
    static Analysis *singleton;

    void ProcessHitCollection(G4VHitsCollection* hc, G4int eventID);
    
    // Pointer to the Detector Geometry
    DetectorConstruction *detector;

};
#endif
