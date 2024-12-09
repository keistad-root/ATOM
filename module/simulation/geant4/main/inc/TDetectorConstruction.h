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
	G4LogicalVolume* alpideMetalLogical = nullptr;
	G4LogicalVolume* alpideEpitaxialLogical = nullptr;
	G4LogicalVolume* alpideSubstrateLogical = nullptr;
	G4LogicalVolume* mCollimatorLogical = nullptr;
	G4LogicalVolume* mScreenLogical = nullptr;

	G4double mAirPressure;
	G4double mCollimatorLength;
	G4double mCollimatorHoleDiameter;
	G4bool mScreenBoolean;

public:
	G4VPhysicalVolume* Construct() override;
	void getWorld();
	void getALPIDE();
	void getCollimator();
	void getScreen();

	G4LogicalVolume* getALPIDEMetal() const { return alpideMetalLogical; }
	G4LogicalVolume* getALPIDEEpitaxial() const { return alpideEpitaxialLogical; }
	G4LogicalVolume* getALPIDESubstrate() const { return alpideSubstrateLogical; }
};

#endif