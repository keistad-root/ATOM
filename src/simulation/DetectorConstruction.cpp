#include "DetectorConstruction.h"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
    DefineCommands();
    material = new Material();
    colour = new Colour();
    solids = new Solid();
    // Construct();
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4bool checkOverlaps = true;

    SetWorld();
    WorldPhysical = new G4PVPlacement(0, G4ThreeVector(), WorldLogical, "World", 0, false, 0, checkOverlaps);
    SetALPIDE();
    G4Transform3D t3d = G4Transform3D();
    ALPIDEAssembly->MakeImprint(WorldLogical, t3d);
    SetCarrierBoard();
    CarrierBoardPhysical = new G4PVPlacement(0, G4ThreeVector(0. * mm, -1.0 * mm * 0.5 + -100 * um * 0.5, -2.8 * mm), CarrierBoardLogical, "CarrierBoard", WorldLogical, false, 0, checkOverlaps);
    
    return WorldPhysical;
}

void DetectorConstruction::SetWorld() {
    G4Box* solidWorld = new G4Box("World", .5 * 300. * mm, .5 * 300. * mm, .5 * 150. * mm);
    WorldLogical = new G4LogicalVolume(solidWorld, material->getWorldMaterial(), "World");
}

void DetectorConstruction::SetAlphaStand() {
    solids->setAlphaStandSolid();
    G4VSolid* solid = solids->getAlphaStandSolid();
    StandLogical = new G4LogicalVolume(solid, material->getStandMaterial(), "StandLogical");
    StandLogical->SetVisAttributes(colour->getStandColour());
}

void DetectorConstruction::SetBetaStand() {
    solids->setBetaStandSolid();
    G4VSolid* solid = solids->getBetaStandSolid();
    StandLogical = new G4LogicalVolume(solid, material->getStandMaterial(), "StandLogical");
    StandLogical->SetVisAttributes(colour->getStandColour());
}

void DetectorConstruction::SetNewStand() {
    solids->setNewStandSolid(cDiameter);
    G4VSolid* solid = solids->getNewStandSolid();
    StandLogical = new G4LogicalVolume(solid, material->getStandMaterial(), "StandLogical");
    StandLogical->SetVisAttributes(colour->getStandColour());
}

void DetectorConstruction::SetShield() {
    solids->setScreenSolid();
    G4VSolid* solid = solids->getScreenSolid();
    ShieldLogical = new G4LogicalVolume(solid, material->getScreenMaterial(), "ShieldLogical");
    ShieldLogical->SetVisAttributes(colour->getScreenColour());
}

void DetectorConstruction::SetALPIDE() {
    G4VSolid* ALPIDECircuitSolid = solids->getAlpideCircuitSolid();
    G4VSolid* ALPIDEEpitaxialSolid = solids->getAlpideEpitaxialSolid();
    
    ALPIDECircuitLogical = new G4LogicalVolume(ALPIDECircuitSolid, material->getAlpideMaterial(), "ALPIDECircuitLogical");
    ALPIDECircuitLogical->SetVisAttributes(colour->getAlpideColour());
    ALPIDECircuitLogical->SetUserLimits(new G4UserLimits(1 * um, 1 * um));
    ALPIDEEpitaxialLogical = new G4LogicalVolume(ALPIDEEpitaxialSolid, material->getAlpideMaterial(), "ALPIDEEpitaxialLogical");
    ALPIDEEpitaxialLogical->SetVisAttributes(colour->getAlpideColour());
    ALPIDEEpitaxialLogical->SetUserLimits(new G4UserLimits(1 * um, 1 * um));

    ALPIDEAssembly = new G4AssemblyVolume();
    G4RotationMatrix * Ra = new G4RotationMatrix(0.0*deg, 0.0*deg, 0.0*deg);
    G4ThreeVector circuitVector = G4ThreeVector(0,  (50.0 - 11.0*0.5) * um, 0);
    ALPIDEAssembly->AddPlacedVolume(ALPIDECircuitLogical,circuitVector, Ra);
    G4ThreeVector epitaxialVector = G4ThreeVector(0, -(11.0) * 0.5 * um, 0);
    ALPIDEAssembly->AddPlacedVolume(ALPIDEEpitaxialLogical, epitaxialVector, Ra);
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

void DetectorConstruction::DefineCommands() {
    fMessenger = new G4GenericMessenger(this, "/Set/Geometry/", "Geometry Control");

    auto& SetSourceTypeCmd = fMessenger->DeclareMethod("SetSourceType", &DetectorConstruction::SetSourceType, "Select source type in alpha, beta, gamma");
    SetSourceTypeCmd.SetParameterName("sourceType", true);
    SetSourceTypeCmd.SetDefaultValue("alpha");
    // 작동
    auto& SetEnergyCmd = fMessenger->DeclareMethodWithUnit("SetEnergy", "MeV", &DetectorConstruction::SetEnergy, "Set energy of particle");
    SetEnergyCmd.SetParameterName("energy", true);
    SetEnergyCmd.SetDefaultValue("5.5");
    // 작동

    auto& SetStandType = fMessenger->DeclareMethod("SetStandType", &DetectorConstruction::SetStandType, "Set stand type");
    SetStandType.SetParameterName("type", true);
    SetStandType.SetDefaultValue("alpha");
    // 작동

    auto& SetDistance = fMessenger->DeclareMethodWithUnit("SetDistance", "mm", &DetectorConstruction::SetDistance, "Set distance from source and ALPIDE");
    SetDistance.SetParameterName("dist", true);
    SetDistance.SetRange("dist>=0. && dist<100.");
    SetDistance.SetDefaultValue("9.");
    // 작동

    auto& SetVacuum = fMessenger->DeclareMethodWithUnit("SetVacuum", "bar", &DetectorConstruction::SetVacuum, "Set density of air");
    SetVacuum.SetParameterName("vac", true);
    SetVacuum.SetRange("vac>=1.e-3 && vac < 1.1");
    SetVacuum.SetDefaultValue("1.");
    // 작동

    auto& SetHallDiameter = fMessenger->DeclareMethodWithUnit("SetHallDiameter", "mm", &DetectorConstruction::SetHallWidth, "Set diameter of hall of source stand");
    SetHallDiameter.SetParameterName("diameter", true);
    // SetHallDiameter.SetRange("diameter>=0. && diameter<5.");
    SetHallDiameter.SetDefaultValue("1.");
}

void DetectorConstruction::SetSourceType(G4String type) {
    if (type == "alpha") sourceType = SourceType::alpha;
    else if (type == "e-") sourceType = SourceType::beta;
    else if (type == "gamma") sourceType = SourceType::gamma;
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/gps/particle " + type);
    UImanager->ApplyCommand("/gps/pos/type Plane");
    UImanager->ApplyCommand("/gps/pos/shape Circle");
    UImanager->ApplyCommand("/gps/pos/radius 2.5 mm");
    UImanager->ApplyCommand("/gps/pos/rot1 0. 0. 1.");
    UImanager->ApplyCommand("/gps/pos/rot2 1. 0. 0.");
    UImanager->ApplyCommand("/gps/ang/type iso");
    UImanager->ApplyCommand("/gps/ene/type Mono");
    UImanager->ApplyCommand("/gps/pos/mono " + std::to_string(sEnergy) + " MeV");
    UImanager->ApplyCommand("/gps/pos/centre 0. " + std::to_string(sDistance) + " 0. mm");
}

void DetectorConstruction::SetEnergy(G4double energy) {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    std::ostringstream command;
    sEnergy = energy;
    command << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
    UImanager->ApplyCommand(command.str());
}

void DetectorConstruction::SetStandType(G4String type) {
    G4bool checkOverlaps = true;

    if (type == "alpha_no_screen") standType = StandType::alpha_no_screen;
    else if (type == "alpha_screen") standType = StandType::alpha_screen;
    else if (type == "beta_no_screen") standType = StandType::beta_no_screen;
    else if (type == "beta_screen") standType = StandType::beta_screen;
    else if (type == "alpha_new_no_screen") standType = StandType::alpha_new_no_screen;
    else if (type == "alpha_new_screen") standType = StandType::alpha_new_screen;

    switch(standType) {
        case StandType::none:
            break;
        case StandType::alpha_no_screen:
            SetAlphaStand();
            StandPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance - 1.) * mm, 0. * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
            break;
        case StandType::alpha_screen:
            SetAlphaStand();
            StandPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance - 1.) * mm, 0. * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
            SetShield();
            ShieldPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance - 1. - 4. - 0.5 * 0.108) * mm, 0. * mm), ShieldLogical, "ShieldLogical", WorldLogical, false, 0, checkOverlaps);
            break;
        case StandType::beta_no_screen:
            SetBetaStand();
            StandPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance - 52.5) * mm, 0. * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
            break;
        case StandType::beta_screen:
            SetBetaStand();
            StandPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance - 52.5) * mm, 0. * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
            SetShield();
            ShieldPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance - 56. - 0.5 * 0.108) * mm, 0. * mm), ShieldLogical, "ShieldLogical", WorldLogical, false, 0, checkOverlaps);
            break;
        case StandType::alpha_new_no_screen:
            SetNewStand();
            StandPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance + 2) * mm, -1.25 * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
            break;
        case StandType::alpha_new_screen:
            SetNewStand();
            StandPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance + 2) * mm, -1.25 * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
            SetShield();
            ShieldPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, (sDistance - 1. - .5 * 0.108) * mm, -1.25 * mm), ShieldLogical, "ShieldLogical", WorldLogical, false, 0, checkOverlaps);
            break;
    }
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetDistance(G4double distance) {
    sDistance = distance;
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/gps/pos/centre 0. " + std::to_string(sDistance) + " 0. mm");
    switch(standType) {
        case StandType::none:
            break;
        case StandType::alpha_no_screen:
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1) * mm, 0. * mm));
            break;
        case StandType::alpha_screen:
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1) * mm, 0. * mm));
            ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1. - 4. - 0.5 * 0.108) * mm, 0. * mm));
            break;
        case StandType::beta_no_screen:
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 52.5) * mm, 0. * mm));
            break;
        case StandType::beta_screen:
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 52.5) * mm, 0. * mm));
            ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 56. - 0.5 * 0.108) * mm, 0. * mm));
            break;
        case StandType::alpha_new_no_screen:
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance + 2) * mm, -1.25 * mm));
            break;
        case StandType::alpha_new_screen:
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance + 2) * mm, -1.25 * mm));
            ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1. - .5 * 0.108) * mm, -1.25 * mm));
            break;
    }
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetVacuum(G4double vac) {
    material->setWorldMaterial(vac/bar);
    G4Material* world_mat = material->getWorldMaterial();
    WorldLogical->SetMaterial(world_mat);
    WorldPhysical->SetLogicalVolume(WorldLogical);
}

void DetectorConstruction::SetHallWidth(G4double diameter) {
    cDiameter = diameter;
    if (StandPhysical != nullptr && (standType == StandType::alpha_new_no_screen || standType == StandType::alpha_new_screen)) {
        SetNewStand();
        StandPhysical->SetLogicalVolume(StandLogical);
    }
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
