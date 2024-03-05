#include "DetectorConstruction.h"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
    DefineCommands();
    Construct();
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    checkOverlaps = true;

    // G4NistManager* nist = G4NistManager::Instance();

    G4double world_sizeXY = 300.0 * mm;
    G4double world_sizeZ = 150.0 * mm;
    G4double air_density = 1.2929e-03 * g / cm3;
    G4double vacuum = 1.0;
    G4double density = air_density * vacuum; 
    G4int nel;
    G4Material* world_mat = new G4Material("world_mat", density, nel = 2);
    world_mat->AddElement(elN, .7);
    world_mat->AddElement(elO, .3);

    G4Box* solidWorld = new G4Box("World", 0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");   
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);

    SetALPIDE();
    G4Transform3D t3d = G4Transform3D();
    ALPIDEAssembly->MakeImprint(logicWorld, t3d);

    SetCarrierBoard();
    new G4PVPlacement(0, G4ThreeVector(0. * mm, -1.0 * mm * 0.5 + -100 * um * 0.5, -2.8 * mm), CarrierBoardLogical, "CarrierBoard", logicWorld, false, 0, checkOverlaps);
    return physWorld;
}

void DetectorConstruction::SetStand() {

}

void DetectorConstruction::SetALPIDE() {
    G4UserLimits* ALPIDEStepLimit = new G4UserLimits(1 * um, 1 * um);

    G4Box* ALPIDECircuitSolid = new G4Box("ALPIDECircuitSolid", 30.0 * mm * 0.5, 11.0 * um * 0.5, 15.0 * mm * 0.5);
    G4Box* ALPIDEEpitaxialSolid = new G4Box("ALPIDEEpitaxialSolid", 30.0 * mm * 0.5, (50.0 - 11.0) * um * 0.5, 15.0 * mm * 0.5);
    
    ALPIDECircuitLogical = new G4LogicalVolume(ALPIDECircuitSolid, ele.ALPIDESi, "ALPIDECircuitLogical");
    ALPIDECircuitLogical->SetVisAttributes(col.alpide_colour);
    ALPIDECircuitLogical->SetUserLimits(ALPIDEStepLimit);
    ALPIDEEpitaxialLogical = new G4LogicalVolume(ALPIDEEpitaxialSolid, ele.ALPIDESi, "ALPIDEEpitaxialLogical");
    ALPIDEEpitaxialLogical->SetVisAttributes(col.alpide_colour);
    ALPIDEEpitaxialLogical->SetUserLimits(ALPIDEStepLimit);

    ALPIDEAssembly = new G4AssemblyVolume();
    G4RotationMatrix * Ra = new G4RotationMatrix(0.0*deg, 0.0*deg, 0.0*deg);
    G4ThreeVector circuitVector = G4ThreeVector(0,  (50.0 - 11.0*0.5) * um, 0);
    ALPIDEAssembly->AddPlacedVolume(ALPIDECircuitLogical,circuitVector, Ra);
    G4ThreeVector epitaxialVector = G4ThreeVector(0, -(11.0) * 0.5 * um, 0);
    ALPIDEAssembly->AddPlacedVolume(ALPIDEEpitaxialLogical, epitaxialVector, Ra);
}

void DetectorConstruction::SetCarrierBoard() {
    G4Box* CarrierBoardBody = new G4Box("CarrierBoardBody", 70. * mm * 0.5, 1.0 * mm * 0.5, 70. * mm * 0.5);
    G4Box* CarrierBoardEmpty = new G4Box("CarrierBoardEmpty", 31. * mm * 0.5, 3 * mm, 12.8 * mm * 0.5);
    G4SubtractionSolid* CarrierBoardSolid = new G4SubtractionSolid("CarrierBoardSolid", CarrierBoardBody, CarrierBoardEmpty, G4Translate3D(0.,0.,2.7 * mm));
    CarrierBoardLogical = new G4LogicalVolume(CarrierBoardSolid, ele.PCB, "CarrierBoardLogical");
    CarrierBoardLogical->SetVisAttributes(col.pcb_colour);
}

G4LogicalVolume* DetectorConstruction::GetScoringStand() const { return StandLogical; }

G4LogicalVolume* DetectorConstruction::GetScoringStand() {
    return StandLogical;
}

G4LogicalVolume* DetectorConstruction::GetScoringALPIDECircuit() {
    return ALPIDECircuitLogical;
}

G4LogicalVolume* DetectorConstruction::GetScoringALPIDEEpitaxial() {
    return ALPIDEEpitaxialLogical;
}

G4LogicalVolume* DetectorConstruction::GetScoringCarrierBoard() {
    return CarrierBoardLogical;
}



void DetectorConstruction::SetVacuum(G4double vac) {
    
    vacuum=vac;
}

void DetectorConstruction::SetAlpha(G4double energy) {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    std::ostringstream stringStream;
    std::ostringstream positionStream;
    stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
    positionStream << "/gps/pos/centre 0. " << SAdistance/mm << " 0. "<< G4String("mm");
    std::vector<G4String> commands = {
        "/gps/particle alpha",
        "/gps/pos/type Plane",
        "/gps/pos/shape Circle",
        "/gps/pos/radius 10. mm",
        "/gps/pos/rot1 0. 0. 1.",
        "/gps/pos/rot2 1. 0. 0.",
        "/gps/ang/type iso",
        "/gps/ene/type Mono",
        G4String(stringStream.str())
    };
    for(G4String command : commands){
        UImanager->ApplyCommand(command);
    }
    UImanager->ApplyCommand(G4String(positionStream.str()));

}

void DetectorConstruction::SetBeta(G4double energy) {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    std::ostringstream stringStream;
    std::ostringstream positionStream;
    stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
    positionStream << "/gps/pos/centre 0. " << SAdistance/mm << " 0. "<< G4String("mm");
    std::vector<G4String> commands = {
        "/gps/particle e-",
        "/gps/pos/type Plane",
        "/gps/pos/shape Circle",
        "/gps/pos/radius 2.5 mm",
        "/gps/pos/rot1 0. 0. 1.",
        "/gps/pos/rot2 1. 0. 0.",
        "/gps/ang/type iso",
        "/gps/ene/type Mono",
        G4String(stringStream.str()),
        G4String(positionStream.str())
    };
    for(G4String command : commands){
        UImanager->ApplyCommand(command);
    }
}

void DetectorConstruction::SetGamma(G4double energy) {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    std::ostringstream stringStream;
    std::ostringstream positionStream;
    stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
    positionStream << "/gps/pos/centre 0. " << SAdistance/mm << " 0. "<< G4String("mm");
    std::vector<G4String> commands = {
        "/gps/particle gamma",
        "/gps/pos/type Plane",
        "/gps/pos/shape Circle",
        "/gps/pos/radius 2.5 mm",
        "/gps/pos/rot1 0. 0. 1.",
        "/gps/pos/rot2 1. 0. 0.",
        "/gps/ang/type iso",
        "/gps/ene/type Mono",
        G4String(stringStream.str()),
        G4String(positionStream.str())
    };
    for(G4String command : commands){
        UImanager->ApplyCommand(command);
    }
}

void DetectorConstruction::SetStandType(G4String type) {
    if (type == "alpha_no_screen") {
        SetAlphaStand();
    } else if (type == "alpha_screen") {
        SetAlphaStand();
        SetShield();
    } else if (type == "beta") {
        SetBetaStand();
    }
}

void DetectorConstruction::SetDistance(G4double distance) {
    SAdistance = distance;
}

void DetectorConstruction::DefineCommands() {
    fMessenger = new G4GenericMessenger(this, "/Set/Geometry/", "Geometry Control");

    auto& SetAlphaCmd = fMessenger->DeclareMethodWithUnit("SetAlpha", "MeV", &DetectorConstruction::SetAlpha, "Set alpha particle energy");
    SetAlphaCmd.SetParameterName("AE", true);
    SetAlphaCmd.SetRange("AE>=0. && AE<10.");
    SetAlphaCmd.SetDefaultValue("5.4");

    auto& SetBetaCmd = fMessenger->DeclareMethodWithUnit("SetBeta", "MeV", &DetectorConstruction::SetBeta, "Set beta energy");
    SetBetaCmd.SetParameterName("BE", true);
    SetBetaCmd.SetRange("BE>=0. && BE<5.");
    SetBetaCmd.SetDefaultValue("0.5");

    auto& SetGammaCmd = fMessenger->DeclareMethodWithUnit("SetGamma", "keV", &DetectorConstruction::SetGamma, "Set gamma energy");
    SetGammaCmd.SetParameterName("GE", true);
    SetGammaCmd.SetRange("GE>=0. && GE<100.");
    SetGammaCmd.SetDefaultValue("5.9");

    auto& SetStandType = fMessenger->DeclareMethod("SetStandType", &DetectorConstruction::SetStandType, "Set stand type");
    SetStandType.SetParameterName("type", true);
    SetStandType.SetDefaultValue("alpha");

    auto& SetDistance = fMessenger->DeclareMethodWithUnit("SetDistance", "mm", &DetectorConstruction::SetDistance, "Set distance from source and ALPIDE");
    SetDistance.SetParameterName("dist", true);
    SetDistance.SetRange("dist>=0. && dist<100.");
    SetDistance.SetDefaultValue("9.");
}