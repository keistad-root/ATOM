#ifndef __TDETECTORCONSTRUCTION__
#define __TDETECTORCONSTRUCTION__

#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"

class TDetectorConstruction : public G4VUserDetectorConstruction {
public:
	TDetectorConstruction() = default;
	~TDetectorConstruction() override = default;
private:
	G4VPhysicalVolume* mWorld = nullptr;
	G4AssemblyVolume* mALPIDE = nullptr;

	G4LogicalVolume* mWorldLogical = nullptr;
	G4LogicalVolume* mALPIDELogical = nullptr;

public:
	G4VPhysicalVolume* Construct() override;
	void getWorld();
	void getALPIDE();

	G4Material* getALPIDEMaterial();
};

#endif