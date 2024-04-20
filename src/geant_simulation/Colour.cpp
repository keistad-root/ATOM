#define __COLOUR_HEADER__
#include "Colour.h"

Colour::Colour() {
	setStandColour();
	setScreenColour();
	setAlpideColour();
	setBoardColour();
}

void Colour::setStandColour() {
	standColour = new G4VisAttributes(G4Colour(1., 1., 1.));
	standColour->SetVisibility(true);
	standColour->SetForceSolid(true);
}

void Colour::setScreenColour() {
	screenColour = new G4VisAttributes(G4Colour(211 / 255., 211 / 255., 211 / 255.));
	screenColour->SetVisibility(true);
	screenColour->SetForceSolid(true);
}

void Colour::setAlpideColour() {
	alpideColour = new G4VisAttributes(G4Colour(1., 1., 0.));
	alpideColour->SetVisibility(true);
	alpideColour->SetForceSolid(true);
}

void Colour::setBoardColour() {
	boardColour = new G4VisAttributes(G4Colour(0 / 255., 100 / 255., 0 / 255.));
	boardColour->SetVisibility(true);
	boardColour->SetForceSolid(true);
}

G4VisAttributes* Colour::getStandColour() const {
	return standColour;
}

G4VisAttributes* Colour::getScreenColour() const {
	return screenColour;
}

G4VisAttributes* Colour::getAlpideColour() const {
	return alpideColour;
}

G4VisAttributes* Colour::getBoardColour() const {
	return boardColour;
}