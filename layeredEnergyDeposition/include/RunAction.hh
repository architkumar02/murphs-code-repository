/**
 * @file    RunAction.hh
 *
 * @date    5 July 2012
 * @author  murffer
 *
 * @brief   Implements user class RunAction
 */

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "Analysis.hh"

class G4Run;

class RunAction : public G4UserRunAction{
    public:
        RunAction(Analysis *a);
        virtual ~RunAction() {};
        virtual void BeginOfRunAction(const G4Run*);
        virtual void   EndOfRunAction(const G4Run*);
    private:
        Analysis *analysis;
};
#endif
