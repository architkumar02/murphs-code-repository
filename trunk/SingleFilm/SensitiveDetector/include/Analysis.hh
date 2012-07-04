#ifndef Analysis_h
#define Analysis_h 1

#include "g4analysis_defs.hh"
#include "G4Event.hh"
#include "G4Run.hh"

// How do we want to save the results?
using namespace G4Root;
//using namespace G4Xml;

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

    // Event Accumulation
    void AddEDepGap(G4int layer, G4double edep) 
        {thisEventTotEGap[layer] += edep;};
    void AddEDepAbs(G4int layer, G4double edep) 
        {thisEventTotEAbs[layer] += edep;};
    void AddSecondary(G4int num) {thisEventSecondaries += num;};
    void AddGammas(G4int numGammas) {thisEventNumGammas += numGammas;};
    void AddElectrons(G4int numE) { thisEventNumElectrons += numE;};
private:
    // Singleton Analysis
    Analysis();
    static Analysis *singleton;

    // Fields
    G4double thisEventTotEGap[NUMLAYERS];   // E Dep in each layer of gap
    G4double thisEventTotEAbs[NUMLAYERS];   // E Dep in each layer of Abs
    
    G4double thisRunTotEGap[NUMLAYERS];   // E Dep in each layer of gap
    G4double thisRunTotEAbs[NUMLAYERS];   // E Dep in each layer of Abs

    G4int thisEventSecondaries;
    G4int thisRunSecondaries;

    G4int thisEventNumGammas;
    G4int thisRunNumGammas;

    G4int thisEventNumElectrons;
    G4int thisRunNumElectrons;
};
#endif
