#ifndef __DETECTORCONSTRUCTION__
#define __DETECTORCONSTRUCTION__

#include "G4VUserDetectorConstruction.hh"
#include "G4GenericMessenger.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"
#include "G4UImanager.hh"

#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"

#include "G4PVPlacement.hh"

#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

#include "Material.h"
#include "Colour.h"
#include "Solid.h"

enum SourceType {
    alpha,
    beta,
    gamma
};

enum StandType {
    alpha_no_screen,
    alpha_screen,
    beta_no_screen,
    beta_screen,
    alpha_new_no_screen,
    alpha_new_screen,
    none
};

class DetectorConstruction : public G4VUserDetectorConstruction {
private:
    G4GenericMessenger* fMessenger;

    Material* material;
    Colour* colour;
    Solid* solids;

    G4VPhysicalVolume* WorldPhysical;
    G4VPhysicalVolume* StandPhysical = nullptr;
    G4VPhysicalVolume* ShieldPhysical = nullptr;
    G4VPhysicalVolume* CarrierBoardPhysical;

    G4LogicalVolume* WorldLogical;
    G4LogicalVolume* StandLogical;
    G4LogicalVolume* ShieldLogical;
    G4LogicalVolume* ALPIDECircuitLogical;
    G4LogicalVolume* ALPIDEEpitaxialLogical;
    G4AssemblyVolume* ALPIDEAssembly;
    G4LogicalVolume* CarrierBoardLogical;

    SourceType sourceType;
    StandType standType = StandType::none;
    G4double sEnergy = 5.4;
    G4double sDistance = 10.;
    G4double cDiameter = 1.;
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

    // Setter
    void SetWorld();
    void SetAlphaStand();
    void SetBetaStand();
    void SetNewStand();
    void SetShield();
    void SetALPIDE();
    void SetCarrierBoard();
    
    // Getter
    G4LogicalVolume* GetScoringStand() const;
    G4LogicalVolume* GetScoringShield() const;
    G4LogicalVolume* GetScoringALPIDECircuit() const;
    G4LogicalVolume* GetScoringALPIDEEpitaxial() const;
    G4LogicalVolume* GetScoringCarrierBoard() const;

    void SetEnergy(G4double energy);
    void SetSourceType(G4String type);
    void SetStandType(G4String type);
    void SetDistance(G4double distance);
    void SetVacuum(G4double vacuum);
    void SetHallWidth(G4double diameter);
private:
    void DefineCommands();
};

#endif