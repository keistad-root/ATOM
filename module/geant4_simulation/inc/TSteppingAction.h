#ifndef __TSTEPPINGACTION__
#define __TSTEPPINGACTION__

#include "G4UserSteppingAction.hh"

#include "TDetectorConstruction.h"

#include "TEventAction.h"

class TSteppingAction : public G4UserSteppingAction {
public:
	TSteppingAction(TEventAction* eventAction);
	~TSteppingAction() override = default;
private:
	TEventAction* fEventAction = nullptr;

	G4LogicalVolume* mALPIDEMetalLogical = nullptr;
	G4LogicalVolume* mALPIDEEpitaxialLogical = nullptr;
	G4LogicalVolume* mALPIDESubstrateLogical = nullptr;
public:
	void UserSteppingAction(const G4Step*) override;

	G4bool isOriginOutALPIDE(const G4Track* track);
};

#endif