#include "TDetectorConstruction.h"

const G4double AIR_DENSITY = 1.2929e-03 * g / cm3;
const G4double N_DENSITY = 14.01 * g / mole;
const G4double O_DENSITY = 16.00 * g / mole;

TDetectorConstruction::TDetectorConstruction(const CppConfigDictionary& config) : G4VUserDetectorConstruction() {
	mAirPressure = stod(config.find("AIR_PRESSURE")) * 0.001;
	mCollimatorLength = stod(config.find("COLLIMATOR_LENGTH")) * mm;
	mCollimatorHoleDiameter = 2 * sqrt(stod(config.find("COLLIMATOR_AREA")) / CLHEP::pi) * mm;
	mDistanceBetweenALPIDEAndCollimator = config.hasKey("DISTANCE_ALPIDE_COLLIMATOR") ? stod(config.find("DISTANCE_ALPIDE_COLLIMATOR")) * mm : 2. * mm;
	mDistanceBetweenSourceAndCollimator = config.hasKey("DISTANCE_SOURCE_COLLIMATOR") ? stod(config.find("DISTANCE_SOURCE_COLLIMATOR")) * mm : .5 * mm;
	if ( config.hasKey("AL_SCREEN") && config.find("AL_SCREEN") == "true" ) {
		mScreenBoolean = true;
	} else if ( config.hasKey("AL_SCREEN") && config.find("AL_SCREEN") == "false" ) {
		mScreenBoolean = false;
	} else {
		mScreenBoolean = true;
	}
	mALPIDEposX = stod(config.find("ALPIDE_POSITION_X")) * mm;
	mALPIDEposY = stod(config.find("ALPIDE_POSITION_Y")) * mm;

	mALPIDEangX = stod(config.find("ALPIDE_ANGLE_X")) * deg;
	mALPIDEangY = stod(config.find("ALPIDE_ANGLE_Y")) * deg;
	mALPIDEangZ = stod(config.find("ALPIDE_ANGLE_Z")) * deg;
}

G4VPhysicalVolume* TDetectorConstruction::Construct() {
	getWorld();
	getALPIDE();
	getCollimator();
	getScreen();

	return mWorld;
}

void TDetectorConstruction::getWorld() {
	G4double worldX = 50 * mm;
	G4double worldY = 50 * mm;
	G4double worldZ = 100 * mm;

	G4Box* solidWorld = new G4Box("World", .5 * worldX, .5 * worldY, .5 * worldZ);

	G4Material* worldMaterial = new G4Material("worldMateiral", mAirPressure * AIR_DENSITY, 2);
	G4Element* elN = new G4Element("Nitrogen", "N", 7, N_DENSITY);
	G4Element* elO = new G4Element("Oxygen", "O", 8, O_DENSITY);
	worldMaterial->AddElement(elN, .7);
	worldMaterial->AddElement(elO, .3);

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

	mAlpideMetalLogical = new G4LogicalVolume(solidALPIDEMetal, alpideMaterial, "ALPIDEMetal");
	mAlpideMetalLogical->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));

	mAlpideEpitaxialLogical = new G4LogicalVolume(solidALPIDEEpitaxial, alpideMaterial, "ALPIDEEpitaxial");
	mAlpideEpitaxialLogical->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));

	mAlpideSubstrateLogical = new G4LogicalVolume(solidALPIDESubstrate, alpideMaterial, "ALPIDESubstrate");
	mAlpideSubstrateLogical->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));

	mALPIDE = new G4AssemblyVolume();


	G4ThreeVector alpideMetalVector = G4ThreeVector(0, 0, -.5 * alpideMetalZ);
	mALPIDE->AddPlacedVolume(mAlpideMetalLogical, alpideMetalVector, new G4RotationMatrix());

	G4ThreeVector alpideEpitaxialVector = G4ThreeVector(0, 0, -alpideMetalZ - .5 * alpideEpitaxialZ);
	mALPIDE->AddPlacedVolume(mAlpideEpitaxialLogical, alpideEpitaxialVector, new G4RotationMatrix());

	G4ThreeVector alpideSubstrateVector = G4ThreeVector(0, 0, -alpideMetalZ - alpideEpitaxialZ - .5 * alpideSubstrateZ);
	mALPIDE->AddPlacedVolume(mAlpideSubstrateLogical, alpideSubstrateVector, new G4RotationMatrix());

	G4ThreeVector alpidePosition = G4ThreeVector(mALPIDEposX, mALPIDEposY, 0);
	G4RotationMatrix* alpideRotation = new G4RotationMatrix();
	alpideRotation->rotateX(mALPIDEangX);
	alpideRotation->rotateY(mALPIDEangY);
	alpideRotation->rotateZ(mALPIDEangZ);
	G4Transform3D t3d = G4Transform3D(*alpideRotation, alpidePosition);
	mALPIDE->MakeImprint(mWorldLogical, t3d);

	G4Region* alpideRegion = new G4Region("ALPIDERegion");
	mAlpideMetalLogical->SetRegion(alpideRegion);
	alpideRegion->AddRootLogicalVolume(mAlpideMetalLogical);
	mAlpideEpitaxialLogical->SetRegion(alpideRegion);
	alpideRegion->AddRootLogicalVolume(mAlpideEpitaxialLogical);
	mAlpideSubstrateLogical->SetRegion(alpideRegion);
	alpideRegion->AddRootLogicalVolume(mAlpideSubstrateLogical);
}

void TDetectorConstruction::getCollimator() {
	G4double collimatorBoxX = 15. * mm;
	G4double collimatorBoxY = 15. * mm;
	G4double collimatorSourceHeight = 5. * mm;
	G4double collimatorSourceDiameter = 13. * mm;

	G4Box* solidCollimatorBox = new G4Box("CollimatorBox", .5 * collimatorBoxX, .5 * collimatorBoxY, .5 * (mCollimatorLength + collimatorSourceHeight));

	G4Tubs* solidCollimatorSource = new G4Tubs("CollimatorSource", 0, .5 * collimatorSourceDiameter, .5 * collimatorSourceHeight, 0, 360 * deg);

	G4Tubs* solidCollimatorHole = new G4Tubs("CollimatorHole", 0, .5 * mCollimatorHoleDiameter, .5 * (mCollimatorLength + collimatorSourceHeight), 0, 360 * deg);

	G4VSolid* temp = new G4SubtractionSolid("temp", solidCollimatorBox, solidCollimatorSource, 0, G4ThreeVector(0, 0, .5 * mCollimatorLength));
	G4VSolid* solidCollimator = new G4SubtractionSolid("Collimator", temp, solidCollimatorHole, 0, G4ThreeVector(0, 0, 0));

	G4Element* elC = new G4Element("Carbon", "C", 6, 12.011 * g / mole);
	G4Element* elH = new G4Element("Hydrogen", "H", 1, 1.008 * g / mole);
	G4Element* elO = new G4Element("Oxygen", "O", 8, 16.00 * g / mole);

	G4Material* collimatorMaterial = new G4Material("CollimatorMaterial", 0.3 * 1.210 * g / cm3, 3);
	collimatorMaterial->AddElement(elC, 3);
	collimatorMaterial->AddElement(elH, 4);
	collimatorMaterial->AddElement(elO, 2);

	mCollimatorLogical = new G4LogicalVolume(solidCollimator, collimatorMaterial, "Collimator");

	mCollimator = new G4PVPlacement(0, G4ThreeVector(0, 0, mDistanceBetweenALPIDEAndCollimator + .5 * (mCollimatorLength + collimatorSourceHeight)), mCollimatorLogical, "Collimator", mWorldLogical, false, 0, true);

	G4Region* collimatorRegion = new G4Region("CollimatorRegion");
	mCollimatorLogical->SetRegion(collimatorRegion);
	collimatorRegion->AddRootLogicalVolume(mCollimatorLogical);
}

void TDetectorConstruction::getScreen() {
	G4double screenX = 15. * mm;
	G4double screenY = 15. * mm;
	G4double screenZ = 100 * um;

	G4Box* solidScreen = new G4Box("Screen", .5 * screenX, .5 * screenY, .5 * screenZ);

	G4Element* elAl = new G4Element("Aluminium", "Al", 13, 26.982 * g / mole);

	G4Material* screenMaterial = new G4Material("ScreenMaterial", 2.7 * g / cm3, 1);
	screenMaterial->AddElement(elAl, 1);

	mScreenLogical = new G4LogicalVolume(solidScreen, screenMaterial, "Screen");

	mScreenLogical->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));

	if ( mScreenBoolean ) {
		mScreen = new G4PVPlacement(0, G4ThreeVector(0, 0, mDistanceBetweenALPIDEAndCollimator - .5 * screenZ), mScreenLogical, "Screen", mWorldLogical, false, 0, true);
	}

	G4Region* screenRegion = new G4Region("ScreenRegion");
	mScreenLogical->SetRegion(screenRegion);
	screenRegion->AddRootLogicalVolume(mScreenLogical);
}