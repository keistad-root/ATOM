#define __DETECTORCONSTRUCTION_HEADER__
#include "DetectorConstruction.h"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
	G4cout << "\033[1;35m" << "Detector Constructor" << "\033[0m" << " is armed" << G4endl;
	material = new Material();
	colour = new Colour();
	solids = new Solid();
}

DetectorConstruction::~DetectorConstruction() {
	delete material;
	delete colour;
	delete solids;
	G4cout << "\033[1;35m" << "Detector Constructor" << "\033[0m" << " is terminated" << G4endl;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
	G4bool checkOverlaps = true;
	if ( WorldLogical != nullptr ) {
		WorldPhysical = new G4PVPlacement(0, G4ThreeVector(), WorldLogical, "World", 0, false, 0, checkOverlaps);
	}
	return WorldPhysical;
}

G4VPhysicalVolume* DetectorConstruction::Construct(G4String standType, G4double distance) {
	G4bool checkOverlaps = true;
	if ( WorldLogical != nullptr ) {
		WorldPhysical = new G4PVPlacement(0, G4ThreeVector(), WorldLogical, "World", 0, false, 0, checkOverlaps);
	}

	if ( StandLogical != nullptr ) {
		if ( standType == "type1" ) {
			StandPhysical = new G4PVPlacement(0, G4ThreeVector(0. * mm, (distance - 1.) * mm, 0. * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
		} else if ( standType == "type2" ) {
			StandPhysical = new G4PVPlacement(0, G4ThreeVector(0. * mm, (distance - 52.5) * mm, 0. * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
		} else if ( standType == "type3" ) {
			StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (distance + 2) * mm, -1.25 * mm));
		}
	}

	if ( ShieldLogical != nullptr ) {
		if ( standType == "type1" ) {
			ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, (distance - 1. - 4. - 0.5 * 0.108) * mm, 0. * mm));
		} else if ( standType == "type2" ) {
			ShieldPhysical = new G4PVPlacement(0, G4ThreeVector(0. * mm, (distance - 56. - 0.5 * 0.108) * mm, 0. * mm), ShieldLogical, "ShieldLogical", WorldLogical, false, 0, checkOverlaps);
		} else if ( standType == "type3" ) {
			ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, (distance - 1. - .5 * 0.108) * mm, -1.25 * mm));
		}
	}

	if ( CarrierBoardLogical != nullptr ) {
		G4Transform3D t3d = G4Transform3D();
		ALPIDEAssembly->MakeImprint(WorldLogical, t3d);
		CarrierBoardPhysical = new G4PVPlacement(0, G4ThreeVector(0. * mm, -1.0 * mm * 0.5 + -100 * um * 0.5, -2.8 * mm), CarrierBoardLogical, "CarrierBoard", WorldLogical, false, 0, !checkOverlaps);
	}

	return WorldPhysical;
}

void DetectorConstruction::SetWorld(G4double air_pressure) {
	material->setWorldMaterial(air_pressure);

	G4double worldX = 300.;
	G4double worldY = 300.;
	G4double worldZ = 500.;

	G4Box* solidWorld = new G4Box("World", .5 * worldX * mm, .5 * worldY * mm, .5 * worldZ * mm);

	WorldLogical = new G4LogicalVolume(solidWorld, material->getWorldMaterial(), "World");
}

void DetectorConstruction::SetStand(G4String standType, G4double hallDiameter) {
	G4VSolid* solid;
	if ( standType == "type1" ) {
		solids->setAlphaStandSolid();
		solid = solids->getAlphaStandSolid();
	} else if ( standType == "type2" ) {
		solids->setBetaStandSolid();
		solid = solids->getBetaStandSolid();
	} else if ( standType == "type3" ) {
		solids->setNewStandSolid(hallDiameter);
		solid = solids->getNewStandSolid();
	} else {
		std::cerr << "\033[1;31m" << "Invalid stand type" << "\033[0m" << std::endl;
		exit(0);
	}
	StandLogical = new G4LogicalVolume(solid, material->getStandMaterial(), "StandLogical");
	StandLogical->SetVisAttributes(colour->getStandColour());
}

void DetectorConstruction::SetShield(G4double shieldWidth) {
	solids->setScreenSolid();
	G4VSolid* solid = solids->getScreenSolid();
	ShieldLogical = new G4LogicalVolume(solid, material->getScreenMaterial(), "ShieldLogical");
	ShieldLogical->SetVisAttributes(colour->getScreenColour());
}

void DetectorConstruction::SetALPIDE(G4String alpideType) {
	if ( alpideType == "insensitive" ) {
		G4VSolid* ALPIDECircuitSolid = solids->getAlpideCircuitSolid();
		G4VSolid* ALPIDEEpitaxialSolid = solids->getAlpideEpitaxialSolid();

		ALPIDECircuitLogical = new G4LogicalVolume(ALPIDECircuitSolid, material->getAlpideMaterial(), "ALPIDECircuitLogical");
		ALPIDECircuitLogical->SetVisAttributes(colour->getAlpideColour());
		ALPIDECircuitLogical->SetUserLimits(new G4UserLimits(1 * um, 1 * um));
		ALPIDEEpitaxialLogical = new G4LogicalVolume(ALPIDEEpitaxialSolid, material->getAlpideMaterial(), "ALPIDEEpitaxialLogical");
		ALPIDEEpitaxialLogical->SetVisAttributes(colour->getAlpideColour());
		ALPIDEEpitaxialLogical->SetUserLimits(new G4UserLimits(1 * um, 1 * um));

		ALPIDEAssembly = new G4AssemblyVolume();
		G4RotationMatrix* Ra = new G4RotationMatrix(0.0 * deg, 0.0 * deg, 0.0 * deg);
		G4ThreeVector circuitVector = G4ThreeVector(0, (50.0 - 11.0 * 0.5) * um, 0);
		ALPIDEAssembly->AddPlacedVolume(ALPIDECircuitLogical, circuitVector, Ra);
		G4ThreeVector epitaxialVector = G4ThreeVector(0, -(11.0) * 0.5 * um, 0);
		ALPIDEAssembly->AddPlacedVolume(ALPIDEEpitaxialLogical, epitaxialVector, Ra);

		SetCarrierBoard();
	} else {
		G4cerr << "\033[1;31m" << "Invalid ALPIDE type" << "\033[0m" << G4endl;
		exit(0);
	}
}

void DetectorConstruction::SetCarrierBoard() {
	G4VSolid* solid = solids->getBoardSolid();
	CarrierBoardLogical = new G4LogicalVolume(solid, material->getBoardMaterial(), "CarrierBoardLogical");
	CarrierBoardLogical->SetVisAttributes(colour->getBoardColour());
}

G4LogicalVolume* DetectorConstruction::GetScoringStand() const {
	return StandLogical;
}

G4LogicalVolume* DetectorConstruction::GetScoringShield() const {
	return ShieldLogical;
}

G4LogicalVolume* DetectorConstruction::GetScoringALPIDECircuit() const {
	return ALPIDECircuitLogical;
}

G4LogicalVolume* DetectorConstruction::GetScoringALPIDEEpitaxial() const {
	return ALPIDEEpitaxialLogical;
}

G4LogicalVolume* DetectorConstruction::GetScoringCarrierBoard() const {
	return CarrierBoardLogical;
}