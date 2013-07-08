/// \file optical//include/RunAction.hh
/// \brief Definition of the RunAction class
//
//
#include "G4UserRunAction.hh"

#ifndef RunAction_h
#define RunAction_h 1

class RecorderBase;

class RunAction : public G4UserRunAction
{
  public:

    RunAction(RecorderBase*);
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  private:

    RecorderBase* fRecorder;
};

#endif
