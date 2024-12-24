#ifndef __TDETECTORCONSTRUCTION__
#define __TDETECTORCONSTRUCTION__

#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4SubtractionSolid.hh"
#include "G4ParticleTable.hh"
#include "G4EmCalculator.hh"

#include "CppConfigFile.h"

class TDetectorConstruction : public G4VUserDetectorConstruction {
public:
	TDetectorConstruction() = default;
	TDetectorConstruction(const CppConfigDictionary& config);
	~TDetectorConstruction() override = default;
private:
	G4VPhysicalVolume* mWorld = nullptr;
	G4AssemblyVolume* mALPIDE = nullptr;
	G4VPhysicalVolume* mCollimator = nullptr;
	G4VPhysicalVolume* mScreen = nullptr;

	G4LogicalVolume* mWorldLogical = nullptr;
	G4LogicalVolume* mAlpideMetalLogical = nullptr;
	G4LogicalVolume* mAlpideEpitaxialLogical = nullptr;
	G4LogicalVolume* mAlpideSubstrateLogical = nullptr;
	G4LogicalVolume* mCollimatorLogical = nullptr;
	G4LogicalVolume* mScreenLogical = nullptr;

	G4double mAirPressure;
	G4double mCollimatorLength;
	G4double mCollimatorHoleDiameter;
	G4double mDistanceBetweenALPIDEAndCollimator;
	G4bool mScreenBoolean;

public:
	G4VPhysicalVolume* Construct() override;
	void getWorld();
	void getALPIDE();
	void getCollimator();
	void getScreen();

	G4LogicalVolume* getALPIDEMetalLogical() const { return mAlpideMetalLogical; }
	G4LogicalVolume* getALPIDEEpitaxialLogical() const { return mAlpideEpitaxialLogical; }
	G4LogicalVolume* getALPIDESubstrateLogical() const { return mAlpideSubstrateLogical; }
	G4LogicalVolume* getCollimatorLogical() const { return mCollimatorLogical; }
	G4LogicalVolume* getScreenLogical() const { return mScreenLogical; }
	G4LogicalVolume* getWorldLogical() const { return mWorldLogical; }
};

#endif