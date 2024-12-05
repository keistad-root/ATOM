#include "TDetectorConstruction.h"

G4VPhysicalVolume* TDetectorConstruction::Construct() {
	getWorld();
	// getALPIDE();

	// G4Transform3D t3d = G4Transform3D();
	// mALPIDE->MakeImprint(mWorldLogical, t3d);

	return mWorld;
}

void TDetectorConstruction::getWorld() {
	G4double worldX = 300 * mm;
	G4double worldY = 300 * mm;
	G4double worldZ = 300 * mm;

	G4Box* solidWorld = new G4Box("World", .5 * worldX, .5 * worldY, .5 * worldZ);

	G4double airPressure = 7.5 * 0.001 * bar;
	G4Material* worldMaterial = new G4Material("Air", 1.29 * mg / cm3, 2, kStateGas, airPressure, 293.15 * kelvin);

	mWorldLogical = new G4LogicalVolume(solidWorld, worldMaterial, "World");

	mWorld = new G4PVPlacement(0, G4ThreeVector(), mWorldLogical, "World", 0, false, 0, true);
}

void TDetectorConstruction::getALPIDE() {
	G4double alpideX = 30. * mm;
	G4double alpideY = 15. * mm;
	G4double alpideMetalZ = 11. * um;
	G4double alpideEpitaxialZ = 25. * um;
	G4double alpideSubstrateZ = 14. * um;

	G4Box* solidALPIDEMetal = new G4Box("ALPIDEMetal", .5 * alpideX, .5 * alpideY, .5 * alpideMetalZ);
	G4Box* solidALPIDEEpitaxial = new G4Box("ALPIDEEpitaxial", .5 * alpideX, .5 * alpideY, .5 * alpideEpitaxialZ);
	G4Box* solidALPIDESubstrate = new G4Box("ALPIDESubstrate", .5 * alpideX, .5 * alpideY, .5 * alpideSubstrateZ);

	G4Material* alpideMaterial = new G4Material("Silicon", 14, 28.085 * g / mole, 2.33 * g / cm3);

	G4LogicalVolume* alpideMetalLogical = new G4LogicalVolume(solidALPIDEMetal, alpideMaterial, "ALPIDEMetal");
	alpideMetalLogical->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
	alpideMetalLogical->SetUserLimits(new G4UserLimits(1 * um, 1 * um));

	G4LogicalVolume* alpideEpitaxialLogical = new G4LogicalVolume(solidALPIDEEpitaxial, alpideMaterial, "ALPIDEEpitaxial");
	alpideEpitaxialLogical->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
	alpideEpitaxialLogical->SetUserLimits(new G4UserLimits(1 * um, 1 * um));

	G4LogicalVolume* alpideSubstrateLogical = new G4LogicalVolume(solidALPIDESubstrate, alpideMaterial, "ALPIDESubstrate");
	alpideSubstrateLogical->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
	alpideSubstrateLogical->SetUserLimits(new G4UserLimits(1 * um, 1 * um));

	mALPIDE = new G4AssemblyVolume();
	G4RotationMatrix* alpideRotation = new G4RotationMatrix(0.0 * deg, 0.0 * deg, 0.0 * deg);
	G4ThreeVector alpideMetalVector = G4ThreeVector(0, 0, -.5 * alpideMetalZ);
	mALPIDE->AddPlacedVolume(alpideMetalLogical, alpideMetalVector, alpideRotation);

	G4ThreeVector alpideEpitaxialVector = G4ThreeVector(0, 0, -alpideMetalZ - .5 * alpideEpitaxialZ);
	mALPIDE->AddPlacedVolume(alpideEpitaxialLogical, alpideEpitaxialVector, alpideRotation);

	G4ThreeVector alpideSubstrateVector = G4ThreeVector(0, 0, -alpideMetalZ - alpideEpitaxialZ - .5 * alpideSubstrateZ);
	mALPIDE->AddPlacedVolume(alpideSubstrateLogical, alpideSubstrateVector, alpideRotation);
}