#include "DetectorConstruction.h"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {
}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    ConstructMaterial();

    G4bool checkOverlaps = true;

    G4double world_sizeXY = 10 * cm;
    G4double world_sizeZ = 5 * cm;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld = new G4Box("World", 
                                  0.5*world_sizeXY, 
                                  0.5 * world_sizeXY, 
                                  0.5 * world_sizeZ);
    
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,
                                                      world_mat,
                                                      "World");
    
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0,
                                                     G4ThreeVector(0, 0, 0),
                                                     logicWorld,
                                                     "World",
                                                     0,
                                                     false,
                                                     0,
                                                     checkOverlaps);
    
    // Define World
}

void DetectorConstruction::ConstructMaterial() {
    elSi = new G4Element("Silicon", "Si", 14, 28.085 * g/mole);
}

void DetectorConstruction::ConstructColors() {
    darkGreen = new G4VisAttributes(G4Colour(0/255., 100/255., 0/255.));
    darkGreen->SetVisibility(true);
    darkGreen->SetForceSolid(true);

    pcb_color = darkGreen;
}

G4LogicalVolume* DetectorConstruction::LV_CarrierBoard() {
    G4Box* APTSPartSolid = new G4Box("APTSCarrierBoardUP_Solid", 7. * cm * 0.5, 7. * cm * 0.5, 1. * cm * 0.5);
    G4Box* APTSInnerSolid = new G4Box("APTSCarrierBoardInner_Solid", 3.1 * cm * 0.5, 1.28 * cm * 0.5, 0.3 * mm);

    G4SubtractionSolid* APTSBoardSolid = new G4SubtractionSolid("APTSCarrierBoard_Solid", APTSPartSolid, APTSInnerSolid, G4Translate3D(0., 2.7*mm, 0.));
    G4LogicalVolume* APTSCarrierBoardLogical = new G4LogicalVolume(APTSBoardSolid, M_PCB(), "APTSCarrierBoard_Logical");
    APTSCarrierBoardLogical->SetVisAttributes(pcb_color);

    return APTSCarrierBoardLogical;
}