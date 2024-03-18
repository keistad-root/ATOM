#ifndef __MATERIAL__
#define __MATERIAL__

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

class Material {
private:
    G4Material* worldMaterial;
    G4Material* standMaterial;
    G4Material* screenMaterial;
    G4Material* alpideMaterial;
    G4Material* boardMaterial;

    G4Material* epoxyResin;
    G4Material* fibrousGlass;

    G4Element* elSi;
    G4Element* elN;
    G4Element* elO;
    G4Element* elAl;
    G4Element* elFe;
    G4Element* elCa;
    G4Element* elMg;
    G4Element* elNa;
    G4Element* elTi;
    G4Element* elC;
    G4Element* elH;
    G4Element* elBr;
public:
    Material();

    G4Material* getWorldMaterial() const;
    G4Material* getStandMaterial() const;
    G4Material* getScreenMaterial() const;
    G4Material* getAlpideMaterial() const;
    G4Material* getBoardMaterial() const;

    void setElement();
    void setWorldMaterial(const G4double density = 1.);
    void setStandMaterial();
    void setScreenMaterial();
    void setAlpideMaterial();
    void setBoardMaterial();
};

#endif