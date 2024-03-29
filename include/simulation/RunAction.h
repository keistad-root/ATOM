#ifndef __RUNACTION__
#define __RUNACTION__

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "PrimaryGeneratorAction.h"
#include "AnalysisManager.h"

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    AnalysisManager* fAnalysisManager;
};

#endif