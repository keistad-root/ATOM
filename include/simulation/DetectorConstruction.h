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

struct Element {
    Element() {
        world_mat = new G4Material("world_mat", 7.5e-03 * 1.2929e-04 * g/cm3, 2);
        world_mat->AddElement(N,.7);
        world_mat->AddElement(O,.3);

        PLA = new G4Material("PLA", 0.3 * 1.210 * g/cm3, 3);
        PLA->AddElement(C, 3);
        PLA->AddElement(H, 4);
        PLA->AddElement(O, 2);

        ALPIDESi = new G4Material("Silicon", 14, 28.085 * g/mole, 2330 * kg/m3);

        FibrousGlass = new G4Material("FibrousGlass", 2.74351 * g/cm3, 7);

        G4Material* SiO2 = new G4Material("SiO2", 2.20 * g/cm3, 2);
        SiO2->AddElement(Si, 1);
        SiO2->AddElement(O, 2);

        G4Material* Al2O3 = new G4Material("Al2O3", 3.97 * g/cm3, 2);
        Al2O3->AddElement(Al, 2);
        Al2O3->AddElement(O, 3);

        G4Material* Fe2O3 = new G4Material("Fe2O3", 5.24 * g/cm3, 2);
        Fe2O3->AddElement(Fe, 2);
        Fe2O3->AddElement(O, 3);
        
        G4Material* CaO = new G4Material("CaO", 3.35 * g/cm3, 2);
        CaO->AddElement(Ca, 1);
        CaO->AddElement(O, 1);

        G4Material* MgO = new G4Material("MgO", 3.58 * g/cm3, 2);
        MgO->AddElement(Mg, 1);
        MgO->AddElement(O, 1);

        G4Material* Na2O = new G4Material("Na2O", 2.27 * g/cm3, 2);
        Na2O->AddElement(Na, 2);
        Na2O->AddElement(O, 1);
        
        G4Material* TiO2 = new G4Material("TiO2", 4.24 * g/cm3, 2);
        TiO2->AddElement(Ti, 1);
        TiO2->AddElement(O, 2);

        FibrousGlass->AddMaterial(SiO2, 60.0 * perCent);
        FibrousGlass->AddMaterial(Al2O3, 11.8 * perCent);
        FibrousGlass->AddMaterial(Fe2O3, 0.1 * perCent);
        FibrousGlass->AddMaterial(CaO, 22.4 * perCent);
        FibrousGlass->AddMaterial(MgO, 3.4 * perCent);
        FibrousGlass->AddMaterial(Na2O, 1.0 * perCent);
        FibrousGlass->AddMaterial(TiO2, 1.3 * perCent);

        EpoxyResin = new G4Material("EpoxyResin", 1.1250 * g/cm3, 4);
        EpoxyResin->AddElement(C, 38);
        EpoxyResin->AddElement(H, 40);
        EpoxyResin->AddElement(O, 6);
        EpoxyResin->AddElement(Br, 4);

        PCB = new G4Material("Fr4", 1.98281 * mg/cm3, 2);
        PCB->AddMaterial(FibrousGlass, 53 * perCent);
        PCB->AddMaterial(EpoxyResin, 47 * perCent);

        Screen = new G4Material("Aluminium", 13, 26.982*g/mole);
    }
    G4Element* Si = new G4Element("Silicon", "Si", 14, 28.085 * g/mole);
    G4Element* N = new G4Element("Nitrogen", "N", 7., 14.007 * g/mole);
    G4Element* O = new G4Element("Oxygen", "O", 8, 15.999 * g/mole);
    G4Element* Al = new G4Element("Aluminium", "Al", 13, 26.982 * g/mole);
    G4Element* Fe = new G4Element("Iron", "Fe", 26, 55.845 * g/mole);
    G4Element* Ca = new G4Element("Calcium", "Ca", 20, 40.078 * g/mole);
    G4Element* Mg = new G4Element("Magnesium", "Mg", 12, 24.305 * g/mole);
    G4Element* Na = new G4Element("Natrium", "Na", 11, 22.990 * g/mole);
    G4Element* Ti = new G4Element("Titanium", "Ti", 22, 47.867 * g/mole);
    G4Element* C = new G4Element("Carbon", "C", 6, 12.011 * g/mole);
    G4Element* H = new G4Element("Hydrogen", "H", 1, 1.008 * g/mole);
    G4Element* Br = new G4Element("Bromine", "Br", 35, 79.904 * g/mole);

    G4Material* world_mat;
    G4Material* PLA;
    G4Material* ALPIDESi;
    G4Material* FibrousGlass;
    G4Material* EpoxyResin;
    G4Material* PCB;
    G4Material* Screen;
};

struct Colour {
    Colour() {
        stand_colour = new G4VisAttributes(G4Colour(1.,1.,1.));
        stand_colour->SetVisibility(true);
        stand_colour->SetForceSolid(true);

        alpide_colour = new G4VisAttributes(G4Colour(1.,1.,0.));
        alpide_colour->SetVisibility(true);
        alpide_colour->SetForceSolid(true);

        pcb_colour = new G4VisAttributes( G4Colour(0/255., 100/255., 0/255.));
        pcb_colour->SetVisibility(true);
        pcb_colour->SetForceSolid(true);

        screen_colour = new G4VisAttributes(G4Colour(211/255., 211/255., 211/255.));
        screen_colour->SetVisibility(true);
        screen_colour->SetForceSolid(true);
    }
    G4VisAttributes* stand_colour;
    G4VisAttributes* alpide_colour;
    G4VisAttributes* pcb_colour;
    G4VisAttributes* screen_colour;
};

class DetectorConstruction : public G4VUserDetectorConstruction {
private:
    G4GenericMessenger* fMessenger;

    Element ele;
    Colour col;

    G4bool checkOverlaps = false;
    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;
    G4LogicalVolume* StandLogical;
    G4LogicalVolume* ShieldLogical;
    G4LogicalVolume* ALPIDECircuitLogical;
    G4LogicalVolume* ALPIDEEpitaxialLogical;
    G4AssemblyVolume* ALPIDEAssembly;
    G4LogicalVolume* CarrierBoardLogical;

    G4double SAdistance = 0;
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

    // Setter
    void SetWorld();
    void SetAlphaStand();
    void SetBetaStand();
    void SetShield();
    void SetALPIDE();
    void SetCarrierBoard();
    
    // Getter
    G4LogicalVolume* GetScoringStand() const;
    G4LogicalVolume* GetShieldStand() const;
    G4LogicalVolume* GetScoringALPIDECircuit() const;
    G4LogicalVolume* GetScoringALPIDEEpitaxial() const;
    G4LogicalVolume* GetScoringCarrierBoard() const;

    void SetAlpha(G4double energy);
    void SetBeta(G4double energy);
    void SetGamma(G4double energy);
    void SetStandType(G4String type);
    void SetDistance(G4double distance);
private:
    void DefineCommands();
};

#endif