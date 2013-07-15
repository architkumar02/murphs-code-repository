#ifndef Analysis_h
#define Analysis_h 1

#include "globals.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"
#include "TH1F.h"
#include "TNtuple.h"
#include "TFile.h"
#include "G4String.hh"

class PMTHit;
class AbsorberHit;
class Analysis {

  public:
    // Singleton Class
    static Analysis* GetInstance(){
      if(Analysis::singleton == NULL)
        Analysis::singleton = new Analysis();
      return Analysis::singleton;
    }

    virtual ~Analysis();

    // Accumulation Methods
    void PrepareNewEvent(const G4Event* anEvent);
    void PrepareNewRun(const G4Run* aRun);
    void EndOfEvent(const G4Event* anEvent);
    void EndOfRun(const G4Run* aRun);
    
    void SetIncidentParticleName(G4String pName);

  private:

    // Singleton Analysis
    Analysis();
    static Analysis *singleton;

    // Class interanls
    G4double GetDetectorThickness();
    G4String GetDetectorMaterial();
    G4String incidentParticleName;
    bool neutron;

    // ROOT Output variables
    TFile* outfile;
};
#endif
