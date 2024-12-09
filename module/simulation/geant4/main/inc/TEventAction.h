#ifndef __TEVENTACTION__
#define __TEVENTACTION__

#include "G4UserEventAction.hh"
#include "TRunAction.h"

#include "TAnalysisManager.h"

class TEventAction : public G4UserEventAction {
public:
	TEventAction(TRunAction* runAction);
	~TEventAction() override = default;

	void BeginOfEventAction(const G4Event*) override;
	void EndOfEventAction(const G4Event*) override;

	void addDepositMetal(G4double depositEnergy) { mDepositMetal += depositEnergy; }
	void addDepositEpitaxial(G4double depositEnergy) { mDepositEpitaxial += depositEnergy; }
	void addDepositSubstrate(G4double depositEnergy) { mDepositSubstrate += depositEnergy; }

	void passMetal() { mMetal = true; }
	void passEpitaxial() { mEpitaxial = true; }
	void passSubstrate() { mSubstrate = true; }
private:
	TRunAction* fRunAction;

	G4bool mMetal = false, mEpitaxial = false, mSubstrate = false;
	G4double mDepositMetal = 0., mDepositEpitaxial = 0., mDepositSubstrate = 0.;
};

#endif