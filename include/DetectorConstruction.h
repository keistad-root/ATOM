#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;

    void ConstructMaterial();
    void ConstructColors();

    G4LogicalVolume* LV_CarrierBoard();

protected:
    G4LogicalVolume* CarrierBoardLogical = nullptr;
    G4LogicalVolume* APTSLogical = nullptr;
    G4LogicalVolume* Stand = nullptr;

private:
    G4Element* elSi;

    G4Material* fMatPCB;

    G4VisAttributes* darkGreen;
    G4VisAttributes* pcb_color;

    G4Material* M_PCB() {return fMatPCB;}
};



#endif