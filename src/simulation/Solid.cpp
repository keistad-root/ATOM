#include "Solid.h"

Solid::Solid() {
    setAlpideCircuitSolid();
    setAlpideEpitaxialSolid();
    setBoardSolid();
}

void Solid::setAlphaStandSolid() {
    G4VSolid* solidBody = new G4Box("solidBody", .5 * 180. * mm, .5 * 8. * mm, .5 * 28.5 * mm);
    G4VSolid* solidSourceHolder = new G4Tubs("solidSourceHolder", 0., 6.5*mm, 3.0*mm, 0., 360. * deg);
    G4VSolid* solidCollimater = new G4Tubs("solidCollimater", 0., 3.0*mm, 8.0*mm, 0., 360.0 * deg);
    
    G4VSolid* subtraction1 = new G4SubtractionSolid("subtraction1", solidBody, solidSourceHolder, new G4RotationMatrix(0. * deg, 90. * deg, 90. * deg), G4ThreeVector(0.0 * mm, 4.0 * mm, 0.0 * mm));
    alphaStandSolid = new G4SubtractionSolid("alphaStandSolid", subtraction1, solidCollimater, new G4RotationMatrix(0. * deg, 90. * deg, 90. * deg), G4ThreeVector(0.0 * mm, 4.0 * mm, 0.0 * mm));
}

void Solid::setBetaStandSolid() {
    G4VSolid* solidBody1 = new G4Box("solidBody1", 70. * mm * 0.5, 7.0 * mm * 0.5, 70. * mm * 0.5);
    G4VSolid* solidBody1Hall1 = new G4Tubs("solidBody1Hall1", 0., 15. * mm * 0.5, 6. * mm * 0.5, 0., 360.0 * deg);
    G4VSolid* solidBody1Hall2 = new G4Tubs("solidBody1Hall2", 0., 13. * mm * 0.5, 7. * mm * 0.5, 0., 360.0 * deg);

    G4VSolid* solidBody2 = new G4Tubs("solidBody2", 2. * mm * 0.5, 30. * mm * 0.5, 59. * mm * 0.5, 0., 360.0 * deg);
    G4VSolid* solidBody2Hall = new G4Tubs("solidBody2Hall", 0., 14. * mm * 0.5, 10.2 * mm * 0.5, 0., 360.0 * deg);

    G4VSolid* subtraction = new G4SubtractionSolid("subtraction", solidBody1, solidBody1Hall1, new G4RotationMatrix(0. * deg, 90. * deg, 0. * deg), G4ThreeVector(0., .5 * mm, 0.));
    G4VSolid* solidStand1 = new G4SubtractionSolid("solidStand1", subtraction, solidBody1Hall2, new G4RotationMatrix(0. * deg, 90. * deg, 0. * deg), G4ThreeVector(0., 0. * mm, 0.));
    G4VSolid* solidStand2 = new G4SubtractionSolid("solidStand2", solidBody2, solidBody2Hall, new G4RotationMatrix(0. * deg, 0. * deg, 0. * deg), G4ThreeVector(0., 0., -24.6 * mm));
    
    betaStandSolid = new G4UnionSolid("betaStandSolid", solidStand1, solidStand2, new G4RotationMatrix(0. * deg, 90. * deg, 0. * deg), G4ThreeVector(0., 33. * mm, 0.));    
}

void Solid::setNewStandSolid(G4double diameter) {
    G4VSolid* solidBody = new G4Box("solidBody", 15. * mm * 0.5, 6. * mm * 0.5, 17.5 * mm * 0.5);
    G4VSolid* solidCollimatedHall = new G4Tubs("solidCollimatedHall", 0., diameter * mm * 0.5, 1.1 * mm * 0.5, 0., 360.0 * deg);
    G4VSolid* solidHolderHall = new G4Tubs("solidHolderHall", 0., 13 * mm * 0.5, 5.2 * mm * 0.5, 0., 360.0 * deg);

    G4VSolid* subtraction = new G4SubtractionSolid("subtraction", solidBody, solidCollimatedHall, new G4RotationMatrix(0. * deg, 90. * deg, 0. * deg), G4ThreeVector(0., -2.5 * mm, 1.25 * mm));
    newStandSolid = new G4SubtractionSolid("newStandSolid", subtraction, solidHolderHall, new G4RotationMatrix(0. * deg, 90. * deg, 0. * deg), G4ThreeVector(0., .59 * mm, 1.25 * mm));
}

void Solid::setScreenSolid() {
    screenSolid = new G4Box("screenSolid", 30. * mm * 0.5, 0.108 * mm * 0.5, 30. * mm * 0.5);
}

void Solid::setAlpideCircuitSolid() {
    alpideCircuitSolid = new G4Box("ALPIDECircuitSolid", 30.0 * mm * 0.5, 11.0 * um * 0.5, 15.0 * mm * 0.5);
}

void Solid::setAlpideEpitaxialSolid() {
    alpideEpitaxialSolid = new G4Box("ALPIDEEpitaxialSolid", 30.0 * mm * 0.5, (50.0 - 11.0) * um * 0.5, 15.0 * mm * 0.5);
}

void Solid::setBoardSolid() {
    G4Box* solidBody = new G4Box("solidBody", 70. * mm * 0.5, 1.0 * mm * 0.5, 70. * mm * 0.5);
    G4Box* solidEmpty = new G4Box("solidEmpty", 31. * mm * 0.5, 3 * mm, 12.8 * mm * 0.5);
    boardSolid = new G4SubtractionSolid("CarrierBoardSolid", solidBody, solidEmpty, G4Translate3D(0.,0.,2.7 * mm));
}

G4VSolid* Solid::getAlphaStandSolid() const {
    return alphaStandSolid;
}

G4VSolid* Solid::getBetaStandSolid() const {
    return betaStandSolid;
}

G4VSolid* Solid::getNewStandSolid() const {
    return newStandSolid;
}

G4VSolid* Solid::getScreenSolid() const {
    return screenSolid;
}

G4VSolid* Solid::getAlpideCircuitSolid() const {
    return alpideCircuitSolid;
}

G4VSolid* Solid::getAlpideEpitaxialSolid() const {
    return alpideEpitaxialSolid;
}

G4VSolid* Solid::getBoardSolid() const {
    return boardSolid;
}