#include "DetectorConstruction.h"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
    material = new Material();
    colour = new Colour();
    solids = new Solid();
    DefineCommands();
    Construct();
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
    new G4PVPlacement(0, G4ThreeVector(0. * mm, -1.0 * mm * 0.5 + -100 * um * 0.5, -2.8 * mm), CarrierBoardLogical, "CarrierBoard", WorldLogical, false, 0, checkOverlaps);
    
    SetAlphaStand();
    StandPhysical = new G4PVPlacement(0,G4ThreeVector(0. * mm, 10 * mm, 0. * mm), StandLogical, "StandLogical", WorldLogical, false, 0, checkOverlaps);
    
    SetShield();
    ShieldPhysical = new G4PVPlacement(0,G4ThreeVector(-10. * mm, -10 * mm, 0. * mm), ShieldLogical, "ShieldLogical", WorldLogical, false, 0, checkOverlaps);
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

    // auto& SetAlphaCmd = fMessenger->DeclareMethodWithUnit("SetAlpha", "MeV", &DetectorConstruction::SetAlpha, "Set alpha particle energy");
    // SetAlphaCmd.SetParameterName("AE", true);
    // SetAlphaCmd.SetRange("AE>=0. && AE<10.");
    // SetAlphaCmd.SetDefaultValue("5.4");

    // auto& SetBetaCmd = fMessenger->DeclareMethodWithUnit("SetBeta", "MeV", &DetectorConstruction::SetBeta, "Set beta energy");
    // SetBetaCmd.SetParameterName("BE", true);
    // SetBetaCmd.SetRange("BE>=0. && BE<5.");
    // SetBetaCmd.SetDefaultValue("0.5");

    // auto& SetGammaCmd = fMessenger->DeclareMethodWithUnit("SetGamma", "keV", &DetectorConstruction::SetGamma, "Set gamma energy");
    // SetGammaCmd.SetParameterName("GE", true);
    // SetGammaCmd.SetRange("GE>=0. && GE<100.");
    // SetGammaCmd.SetDefaultValue("5.9");

    auto& SetStandType = fMessenger->DeclareMethod("SetStandType", &DetectorConstruction::SetStandType, "Set stand type");
    SetStandType.SetParameterName("type", true);
    SetStandType.SetDefaultValue("alpha");

    auto& SetDistance = fMessenger->DeclareMethodWithUnit("SetDistance", "mm", &DetectorConstruction::SetDistance, "Set distance from source and ALPIDE");
    SetDistance.SetParameterName("dist", true);
    SetDistance.SetRange("dist>=0. && dist<100.");
    SetDistance.SetDefaultValue("9.");

    auto& SetVacuum = fMessenger->DeclareMethodWithUnit("SetVacuum", "bar", &DetectorConstruction::SetVacuum, "Set density of air");
    SetVacuum.SetParameterName("vac", true);
    SetVacuum.SetRange("vac>=1.e-3 && vac < 1.1");
    SetVacuum.SetDefaultValue("1.");

    auto& SetHallDiameter = fMessenger->DeclareMethodWithUnit("SetHallDiameter", "mm", &DetectorConstruction::SetHallWidth, "Set diameter of hall of source stand");
    SetHallDiameter.SetParameterName("diameter", true);
    SetHallDiameter.SetRange("diameter>=0. && diameter<4.");
    SetHallDiameter.SetDefaultValue("1.");
}

void DetectorConstruction::SetSourceType(G4String type) {
    if (type == "alpha") sourceType = SourceType::alpha;
    else if (type == "beta") sourceType = SourceType::beta;
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
    command << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
    UImanager->ApplyCommand(command.str());
}

void DetectorConstruction::SetStandType(G4String type) {
    if (type == "alpha_no_screen") standType = StandType::alpha_no_screen;
    else if (type == "alpha_screen") standType = StandType::alpha_screen;
    else if (type == "beta_no_screen") standType = StandType::beta_no_screen;
    else if (type == "beta_screen") standType = StandType::beta_screen;
    else if (type == "alpha_new_no_screen") standType = StandType::alpha_new_no_screen;
    else if (type == "alpha_new_screen") standType = StandType::alpha_new_screen;

    switch(standType) {
        case StandType::alpha_no_screen:
            SetAlphaStand();
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1) * mm, 0. * mm));
            ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, - 10 * mm, 0. * mm));
            break;
        case StandType::alpha_screen:
            SetAlphaStand();
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1) * mm, 0. * mm));
            ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1) * mm, 0. * mm));
            break;
        case StandType::beta_no_screen:
            SetBetaStand();
            StandPhysical->SetLogicalVolume(StandLogical);
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 53.) * mm, 0. * mm));
            break;
        case StandType::beta_screen:

            break;
        case StandType::alpha_new_no_screen:

            break;
        case StandType::alpha_new_screen:
            
            break;
    }
}

void DetectorConstruction::SetDistance(G4double distance) {
    sDistance = distance;
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/gps/pos/centre 0. " + std::to_string(sDistance) + " 0. mm");
    switch(standType) {
        case StandType::alpha_no_screen:
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1) * mm, 0. * mm));
            ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, - 10 * mm, 0. * mm));
            break;
        case StandType::alpha_screen:
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1) * mm, 0. * mm));
            ShieldPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 1) * mm, 0. * mm));
            break;
        case StandType::beta_no_screen:
            StandPhysical->SetLogicalVolume(StandLogical);
            StandPhysical->SetTranslation(G4ThreeVector(0. * mm, (sDistance - 53.) * mm, 0. * mm));
            break;
        case StandType::beta_screen:

            break;
        case StandType::alpha_new_no_screen:

            break;
        case StandType::alpha_new_screen:
            
            break;
    }

}

void DetectorConstruction::SetVacuum(G4double vac) {
    material->setWorldMaterial(vac/bar);
    G4Material* world_mat = material->getWorldMaterial();
    WorldLogical->SetMaterial(world_mat);
    WorldPhysical->SetLogicalVolume(WorldLogical);
}

void DetectorConstruction::SetHallWidth(G4double diameter) {

}


// void DetectorConstruction::SetAlpha(G4double energy) {
//     G4UImanager* UImanager = G4UImanager::GetUIpointer();
//     std::ostringstream stringStream;
//     std::ostringstream positionStream;
//     stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
//     positionStream << "/gps/pos/centre 0. " << SAdistance/mm << " 0. "<< G4String("mm");
//     std::vector<G4String> commands = {
//         "/gps/particle alpha",
//         "/gps/pos/type Plane",
//         "/gps/pos/shape Circle",
//         "/gps/pos/radius 2.5 mm",
//         "/gps/pos/rot1 0. 0. 1.",
//         "/gps/pos/rot2 1. 0. 0.",
//         "/gps/ang/type iso",
//         "/gps/ene/type Mono",
//         G4String(stringStream.str())
//     };
//     for(G4String command : commands){
//         UImanager->ApplyCommand(command);
//     }
//     UImanager->ApplyCommand(G4String(positionStream.str()));
// }

// void DetectorConstruction::SetBeta(G4double energy) {
//     G4UImanager* UImanager = G4UImanager::GetUIpointer();
//     std::ostringstream stringStream;
//     std::ostringstream positionStream;
//     stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
//     positionStream << "/gps/pos/centre 0. " << SAdistance/mm << " 0. "<< G4String("mm");
//     std::vector<G4String> commands = {
//         "/gps/particle e-",
//         "/gps/pos/type Plane",
//         "/gps/pos/shape Circle",
//         "/gps/pos/radius 2.5 mm",
//         "/gps/pos/rot1 0. 0. 1.",
//         "/gps/pos/rot2 1. 0. 0.",
//         "/gps/ang/type iso",
//         "/gps/ene/type Mono",
//         G4String(stringStream.str()),
//         G4String(positionStream.str())
//     };
//     for(G4String command : commands){
//         UImanager->ApplyCommand(command);
//     }
// }

// void DetectorConstruction::SetGamma(G4double energy) {
//     G4UImanager* UImanager = G4UImanager::GetUIpointer();
//     std::ostringstream stringStream;
//     std::ostringstream positionStream;
//     stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
//     positionStream << "/gps/pos/centre 0. " << SAdistance/mm << " 0. "<< G4String("mm");
//     std::vector<G4String> commands = {
//         "/gps/particle gamma",
//         "/gps/pos/type Plane",
//         "/gps/pos/shape Circle",
//         "/gps/pos/radius 2.5 mm",
//         "/gps/pos/rot1 0. 0. 1.",
//         "/gps/pos/rot2 1. 0. 0.",
//         "/gps/ang/type iso",
//         "/gps/ene/type Mono",
//         G4String(stringStream.str()),
//         G4String(positionStream.str())
//     };
//     for(G4String command : commands){
//         UImanager->ApplyCommand(command);
//     }
// }
