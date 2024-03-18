#include "Material.h"

Material::Material() {
    setElement();

    setWorldMaterial();
    setStandMaterial();
    setScreenMaterial();
    setAlpideMaterial();
    setBoardMaterial();
}

void Material::setElement() {
    elSi = new G4Element("Silicon", "Si", 14, 28.085 * g/mole);
    elN = new G4Element("Nitrogen", "N", 7., 14.007 * g/mole);
    elO = new G4Element("Oxygen", "O", 8, 15.999 * g/mole);
    elAl = new G4Element("Aluminium", "Al", 13, 26.982 * g/mole);
    elFe = new G4Element("Iron", "Fe", 26, 55.845 * g/mole);
    elCa = new G4Element("Calcium", "Ca", 20, 40.078 * g/mole);
    elMg = new G4Element("Magnesium", "Mg", 12, 24.305 * g/mole);
    elNa = new G4Element("Natrium", "Na", 11, 22.990 * g/mole);
    elTi = new G4Element("Titanium", "Ti", 22, 47.867 * g/mole);
    elC = new G4Element("Carbon", "C", 6, 12.011 * g/mole);
    elH = new G4Element("Hydrogen", "H", 1, 1.008 * g/mole);
    elBr = new G4Element("Bromine", "Br", 35, 79.904 * g/mole);
}

void Material::setWorldMaterial(const G4double density) {
    worldMaterial = new G4Material("world_mat", density * 1.2929e-03 * g / cm3, 2);
    worldMaterial->AddElement(elN, .7);
    worldMaterial->AddElement(elO, .3);
}

void Material::setStandMaterial() {
    standMaterial = new G4Material("PLA", 0.3 * 1.210 * g/cm3, 3);
    standMaterial->AddElement(elC, 3);
    standMaterial->AddElement(elH, 4);
    standMaterial->AddElement(elO, 2);
}

void Material::setScreenMaterial() {
    screenMaterial = new G4Material("Aluminium", 13, 26.982 * g/mole, 2.7 * g/cm3);
}

void Material::setAlpideMaterial() {
    alpideMaterial = new G4Material("Silicon", 14, 28.085 * g/mole, 2.33 * g/cm3);
}

void Material::setBoardMaterial() {
    fibrousGlass = new G4Material("fibrousGlass", 2.74351 * g/cm3, 7);

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

    fibrousGlass->AddMaterial(SiO2, 60.0 * perCent);
    fibrousGlass->AddMaterial(Al2O3, 11.8 * perCent);
    fibrousGlass->AddMaterial(Fe2O3, 0.1 * perCent);
    fibrousGlass->AddMaterial(CaO, 22.4 * perCent);
    fibrousGlass->AddMaterial(MgO, 3.4 * perCent);
    fibrousGlass->AddMaterial(Na2O, 1.0 * perCent);
    fibrousGlass->AddMaterial(TiO2, 1.3 * perCent);

    epoxyResin = new G4Material("epoxyResin", 1.1250 * g/cm3, 4);
    epoxyResin->AddElement(elC, 38);
    epoxyResin->AddElement(elH, 40);
    epoxyResin->AddElement(elO, 6);
    epoxyResin->AddElement(elBr, 4);

    boardMaterial = new G4Material("Fr4", 1.98281 * mg/cm3, 2);
    boardMaterial->AddMaterial(fibrousGlass, 53 * perCent);
    boardMaterial->AddMaterial(epoxyResin, 47 * perCent);
}

G4Material* Material::getWorldMaterial()  const {
    return worldMaterial;
}

G4Material* Material::getStandMaterial()  const {
    return standMaterial;
}

G4Material* Material::getScreenMaterial()  const {
    return screenMaterial;
}

G4Material* Material::getAlpideMaterial()  const {
    return alpideMaterial;
}

G4Material* Material::getBoardMaterial()  const {
    return boardMaterial;
}