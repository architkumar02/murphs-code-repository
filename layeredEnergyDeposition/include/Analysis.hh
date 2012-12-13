#ifndef Analysis_h
#define Analysis_h 1

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"
#include "DetectorConstruction.hh"

#include "CaloHit.hh"

#include "globals.hh"

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

    // Pointer to the Detector Geometry
    DetectorConstruction *detector;

    // Accumulation Variables
    G4int numSlices;
    G4double *muEDepRun;
    G4double *varEDepRun;
    G4int    *numEntries;
    G4double *eDepEvent;
    
};
#endif
