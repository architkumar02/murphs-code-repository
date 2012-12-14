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

        virtual ~Analysis() {};

        // Accumulation Methods
        void PrepareNewEvent(const G4Event* anEvent);
        void PrepareNewRun(const G4Run* aRun);
        void EndOfEvent(const G4Event* anEvent);
        void EndOfRun(const G4Run* aRun);
        
        // Geometry variables
        G4int   GetNumberGapSlices()      {return numberGapSlices;};
        G4int   GetNumberAbsSlices()      {return numberAbsSlices;};
        G4int   GetNumberSlices()         {return numberSlices;};
   
   private:

        void ComputeParameters();

        // Pointer to the Detector Geometry
        DetectorConstruction *detector;

        // Geometry Variables
        G4double sliceThickness;        // Thickness of an energy depostion slice
        G4int   numberGapSlices;        // Number of gap slices
        G4int   numberAbsSlices;        // Number of absorber slices
        G4int   numberSlices;           // Number of slice (total)

        // Accumulation Variables
        G4double *muEDepRun;
        G4double *varEDepRun;
        G4int    *numEntries;
        G4double *eDepEvent;

};
#endif
