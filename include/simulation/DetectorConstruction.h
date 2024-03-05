#ifndef __DETECTORCONSTRUCTION__
#define __DETECTORCONSTRUCTION__

#include "G4VUserDetectorConstruction.hh"
#include "G4GenericMessenger.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4UImanager.hh"

#include "G4AssemblyVolume.hh"

#include "G4PVPlacement.hh"

#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"


class DetectorConstruction : public G4VUserDetectorConstruction {
private:
    G4GenericMessenger* fMessenger;

    G4Element* elSi = new G4Element("Silicon", "Si", 14, 28.085 * g/mole);
    G4Element* elN = new G4Element("Nitrogen", "N", 7., 14.007 * g/mole);
    G4Element* elO = new G4Element("Oxygen", "O", 8, 15.999 * g/mole);
    G4Element* elAl = new G4Element("Aluminium", "Al", 13, 26.982 * g/mole);
    G4Element* elFe = new G4Element("Iron", "Fe", 26, 55.845 * g/mole);
    G4Element* elCa = new G4Element("Calcium", "Ca", 20, 40.078 * g/mole);
    G4Element* elMg = new G4Element("Magnesium", "Mg", 12, 24.305 * g/mole);
    G4Element* elNa = new G4Element("Natrium", "Na", 11, 22.990 * g/mole);
    G4Element* elTi = new G4Element("Titanium", "Ti", 22, 47.867 * g/mole);
    G4Element* elC = new G4Element("Carbon", "C", 6, 12.011 * g/mole);
    G4Element* elH = new G4Element("Hydrogen", "H", 1, 1.008 * g/mole);
    G4Element* elBr = new G4Element("Bromine", "Br", 35, 79.904 * g/mole);

    G4LogicalVolume* Stand1Logical;
    G4LogicalVolume* Stand2Logical;
    G4LogicalVolume* ShieldLogical;
    G4LogicalVolume* ALPIDECircuitLogical;
    G4LogicalVolume* ALPIDEEpitaxialLogical;
    G4AssemblyVolume* ALPIDEAssembly;
    G4LogicalVolume* CarrierBoardLogical;

    G4double vacuum;
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

    // Setter
    void SetStand1();
    void SetStand2();
    void SetShield();
    void SetALPIDE();
    void SetCarrierBoard();
    
    // Getter
    G4LogicalVolume* GetScoringStand2() const;
    G4LogicalVolume* GetScoringALPIDECircuit() const;
    G4LogicalVolume* GetScoringALPIDEEpitaxial() const;
    G4LogicalVolume* GetScoringCarrierBoard() const;

    void SetVacuum(G4double vac);
    void SetAlpha(G4double energy);
    void SetBeta(G4double energy);
    void SetGamma(G4double energy);
private:
    void DefineCommands();
};

#endif