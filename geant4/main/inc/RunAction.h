#ifndef __RUNACTION__
#define __RUNACTION__

#ifdef __RUNACTION_HEADER__
#include "G4ios.hh"
#include "G4Run.hh"
#include "AnalysisManager.h"
#endif

#include "G4UserRunAction.hh"
#include<time.h>
// #include "G4Run.hh"

// #include "PrimaryGeneratorAction.h"
// #include "AnalysisManager.h"
class AnalysisManager;

class RunAction : public G4UserRunAction {
private:
	clock_t mRunStart, mRunFinish;
public:
	RunAction();
	virtual ~RunAction();

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);

	AnalysisManager* fAnalysisManager;
};

#endif