#ifndef Analysis_h
#define Analysis_h 1

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"

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
        
        // Updating geometry
        void ComputeParameters(G4double caloThickness);
        
        // Accumulation Methods
        void PrepareNewEvent(const G4Event* anEvent);
        void PrepareNewRun(const G4Run* aRun);
        void EndOfEvent(const G4Event* anEvent);
        void EndOfRun(const G4Run* aRun);
        
        // Geometry variables
        G4int   GetNumberSlices()         {return numberSlices;};
   
   private:

        // Singleton Analysis
        Analysis();
        static Analysis *singleton;

        // Geometry Variables
        G4double sliceThickness;        // Thickness of an energy depostion slice
        G4double caloThickness;         // Thickness of the calorimeters
        G4int   numberSlices;           // Number of slice (total)

        // Accumulation Variables
        G4double *muEDepRun;
        G4double *varEDepRun;
        G4int    *numEntries;
        G4double *eDepEvent;

};
#endif
