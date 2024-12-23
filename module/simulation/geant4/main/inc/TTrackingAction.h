#ifndef __TTACKINGACTION__
#define __TTACKINGACTION__

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "TDetectorConstruction.h"
#include "G4RunManager.hh"
#include "TAnalysisManager.h"

class TTrackingAction : public G4UserTrackingAction {
public:
	TTrackingAction() = default;
	~TTrackingAction() override = default;

	void PreUserTrackingAction(const G4Track*) override;
	void PostUserTrackingAction(const G4Track*) override;

	void getLogicalVolumes();
private:
	G4double mDepositEnergyMetal = 0.;
	G4double mDepositEnergyEpitaxial = 0.;
	G4double mDepositEnergySubstrate = 0.;

	G4bool hasLogicalVolumes = false;

	G4LogicalVolume* mALPIDEMetalLogical = nullptr;
	G4LogicalVolume* mALPIDEEpitaxialLogical = nullptr;
	G4LogicalVolume* mALPIDESubstrateLogical = nullptr;
	G4LogicalVolume* mCollimatorLogical = nullptr;
	G4LogicalVolume* mScreenLogical = nullptr;
	G4LogicalVolume* mWorldLogical = nullptr;
};

#endif