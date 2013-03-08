#ifndef Analysis_h
#define Analysis_h 1

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"
#include "TH1F.h"
#include "TNtuple.h"
#include "TFile.h"

#include "CaloHit.hh"

#include "globals.hh"

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
   
   private:

        // Singleton Analysis
        Analysis();
        static Analysis *singleton;
        G4double GetDetectorThickness();

        // ROOT Output variables
        TFile* outfile;
        TH1F* eDepHist;
        TNtuple* posEDepTuple;

        // Accumulation Variables
        G4double eDepEvent;

};
#endif
