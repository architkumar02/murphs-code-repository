/// \file optical//include/SteppingVerbose.hh
/// \brief Definition of the SteppingVerbose class
//
//
#ifndef SteppingVerbose_h
#define SteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

class SteppingVerbose : public G4SteppingVerbose
{
  public:

    SteppingVerbose();
    virtual ~SteppingVerbose();

    virtual void StepInfo();
    virtual void TrackingStarted();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
