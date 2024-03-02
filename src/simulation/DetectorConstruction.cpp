#include "DetectorConstruction.h"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
    DefineCommands();
    Construct();
}

DetectorConstruction::~DetectorConstruction() {

}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4bool checkOverlaps = true;

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
    new G4PVPlacement(0, G4ThreeVector(0 * mm, -1.0 * mm * 0.5 + -100 * um * 0.5, -2.8 * mm), CarrierBoardLogical, "CarrierBoard", logicWorld, false, 0, checkOverlaps);

    return physWorld;
}

void DetectorConstruction::SetStand() {

}

void DetectorConstruction::SetALPIDE() {
    G4Material* ALPIDESi = new G4Material("Silicon", 14, 28.085 * g/mole, 2330 * kg/m3);
    G4VisAttributes* alpide_colour = new G4VisAttributes(G4Colour(1.,1.,0.));
    alpide_colour->SetVisibility(true);
    alpide_colour->SetForceSolid(true);
    G4UserLimits* ALPIDEStepLimit = new G4UserLimits(1 * um, 1 * um);

    G4Box* ALPIDECircuitSolid = new G4Box("ALPIDECircuitSolid", 30.0 * mm * 0.5, 11.0 * um * 0.5, 15.0 * mm * 0.5);
    G4Box* ALPIDEEpitaxialSolid = new G4Box("ALPIDEEpitaxialSolid", 30.0 * mm * 0.5, (50.0 - 11.0) * um * 0.5, 15.0 * mm * 0.5);
    
    ALPIDECircuitLogical = new G4LogicalVolume(ALPIDECircuitSolid, ALPIDESi, "ALPIDECircuitLogical");
    ALPIDECircuitLogical->SetVisAttributes(alpide_colour);
    ALPIDECircuitLogical->SetUserLimits(ALPIDEStepLimit);
    ALPIDEEpitaxialLogical = new G4LogicalVolume(ALPIDEEpitaxialSolid, ALPIDESi, "ALPIDEEpitaxialLogical");
    ALPIDEEpitaxialLogical->SetVisAttributes(alpide_colour);
    ALPIDEEpitaxialLogical->SetUserLimits(ALPIDEStepLimit);

    ALPIDEAssembly = new G4AssemblyVolume();
    G4RotationMatrix * Ra = new G4RotationMatrix(0.0*deg, 0.0*deg, 0.0*deg);
    G4ThreeVector circuitVector = G4ThreeVector(0,  (50.0 - 11.0*0.5) * um, 0);
    ALPIDEAssembly->AddPlacedVolume(ALPIDECircuitLogical,circuitVector, Ra);
    G4ThreeVector epitaxialVector = G4ThreeVector(0, -(11.0) * 0.5 * um, 0);
    ALPIDEAssembly->AddPlacedVolume(ALPIDEEpitaxialLogical, epitaxialVector, Ra);
}

void DetectorConstruction::SetCarrierBoard() {
    G4Material* FibrousGlass = new G4Material("FibrousGlass", 2.74351 * g/cm3, 7);

    G4Material* SiO2 = new G4Material("SiO2", 2.20 * g/cm3, 2);
    SiO2->AddElement(elSi, 1);
    SiO2->AddElement(elO, 2);

    G4Material* Al2O3 = new G4Material("Al2O3", 3.97 * g/cm3, 2);
    Al2O3->AddElement(elAl, 2);
    Al2O3->AddElement(elO, 3);

    G4Material* Fe2O3 = new G4Material("Fe2O3", 5.24 * g/cm3, 2);
    Fe2O3->AddElement(elFe, 2);
    Fe2O3->AddElement(elO, 3);
    
    G4Material* CaO = new G4Material("CaO", 3.35 * g/cm3, 2);
    CaO->AddElement(elCa, 1);
    CaO->AddElement(elO, 1);

    G4Material* MgO = new G4Material("MgO", 3.58 * g/cm3, 2);
    MgO->AddElement(elMg, 1);
    MgO->AddElement(elO, 1);

    G4Material* Na2O = new G4Material("Na2O", 2.27 * g/cm3, 2);
    Na2O->AddElement(elNa, 2);
    Na2O->AddElement(elO, 1);
    
    G4Material* TiO2 = new G4Material("TiO2", 4.24 * g/cm3, 2);
    TiO2->AddElement(elTi, 1);
    TiO2->AddElement(elO, 2);

    FibrousGlass->AddMaterial(SiO2, 60.0 * perCent);
    FibrousGlass->AddMaterial(Al2O3, 11.8 * perCent);
    FibrousGlass->AddMaterial(Fe2O3, 0.1 * perCent);
    FibrousGlass->AddMaterial(CaO, 22.4 * perCent);
    FibrousGlass->AddMaterial(MgO, 3.4 * perCent);
    FibrousGlass->AddMaterial(Na2O, 1.0 * perCent);
    FibrousGlass->AddMaterial(TiO2, 1.3 * perCent);

    G4Material* EpoxyResin = new G4Material("EpoxyResin", 1.1250 * g/cm3, 4);
    EpoxyResin->AddElement(elC, 38);
    EpoxyResin->AddElement(elH, 40);
    EpoxyResin->AddElement(elO, 6);
    EpoxyResin->AddElement(elBr, 4);

    G4Material* PCB = new G4Material("Fr4", 1.98281 * mg/cm3, 2);
    PCB->AddMaterial(FibrousGlass, 53 * perCent);
    PCB->AddMaterial(EpoxyResin, 47 * perCent);

    G4Box* CarrierBoardBody = new G4Box("CarrierBoardBody", 70. * mm * 0.5, 1.0 * mm * 0.5, 70. * mm * 0.5);
    G4Box* CarrierBoardEmpty = new G4Box("CarrierBoardEmpty", 31. * mm * 0.5, 3 * mm, 12.8 * mm * 0.5);
    G4SubtractionSolid* CarrierBoardSolid = new G4SubtractionSolid("CarrierBoardSolid", CarrierBoardBody, CarrierBoardEmpty, G4Translate3D(0.,0.,2.7 * mm));
    CarrierBoardLogical = new G4LogicalVolume(CarrierBoardSolid, PCB, "CarrierBoardLogical");
    
    G4VisAttributes* pcb_colour = new G4VisAttributes( G4Colour(0/255., 100/255., 0/255.) );
    pcb_colour->SetVisibility(true);
    pcb_colour->SetForceSolid(true);
    
    CarrierBoardLogical->SetVisAttributes(pcb_colour);
}


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
    stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
    std::vector<G4String> commands = {
        "/gps/particle alpha",
        "/gps/pos/type Plane",
        "/gps/pos/shape Circle",
        "/gps/pos/radius 2.5 mm",
        "/gps/pos/rot1 0. 0. 1.",
        "/gps/pos/rot2 1. 0. 0.",
        "/gps/ang/type iso",
        "/gps/ene/type Mono",
        G4String(stringStream.str())
    };
    for(G4String command : commands){
        UImanager->ApplyCommand(command);
    }
}

void DetectorConstruction::SetBeta(G4double energy) {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    std::ostringstream stringStream;
    stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
    std::vector<G4String> commands = {
        "/gps/particle beta",
        "/gps/pos/type Plane",
        "/gps/pos/shape Circle",
        "/gps/pos/radius 2.5 mm",
        "/gps/pos/rot1 0. 0. 1.",
        "/gps/pos/rot2 1. 0. 0.",
        "/gps/ang/type iso",
        "/gps/ene/type Mono",
        G4String(stringStream.str())
    };
    for(G4String command : commands){
        UImanager->ApplyCommand(command);
    }
}

void DetectorConstruction::SetGamma(G4double energy) {
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    std::ostringstream stringStream;
    stringStream << "/gps/ene/mono " << energy/MeV << G4String(" MeV");
    std::vector<G4String> commands = {
        "/gps/particle gamma",
        "/gps/pos/type Plane",
        "/gps/pos/shape Circle",
        "/gps/pos/radius 2.5 mm",
        "/gps/pos/rot1 0. 0. 1.",
        "/gps/pos/rot2 1. 0. 0.",
        "/gps/ang/type iso",
        "/gps/ene/type Mono",
        G4String(stringStream.str())
    };
    for(G4String command : commands){
        UImanager->ApplyCommand(command);
    }
}


void DetectorConstruction::DefineCommands() {
    fMessenger = new G4GenericMessenger(this, "/Set/Geometry/", "Geometry Control");

    auto& SetVacuumCmd = fMessenger->DeclareMethod("SetVacuum",&DetectorConstruction::SetVacuum, "Set vacuum");
    SetVacuumCmd.SetParameterName("vac", true);
    SetVacuumCmd.SetDefaultValue("1.");

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

}