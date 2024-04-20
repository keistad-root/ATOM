#ifndef __COLOUR__
#define __COLOUR__

#ifdef __COLOUR_HEADER__
#include "G4VisAttributes.hh"
#endif

class G4VisAttributes;

class Colour {
private:
	G4VisAttributes* standColour;
	G4VisAttributes* screenColour;
	G4VisAttributes* alpideColour;
	G4VisAttributes* boardColour;
public:
	Colour();

	void setStandColour();
	void setScreenColour();
	void setAlpideColour();
	void setBoardColour();

	G4VisAttributes* getStandColour() const;
	G4VisAttributes* getScreenColour() const;
	G4VisAttributes* getAlpideColour() const;
	G4VisAttributes* getBoardColour() const;
};

#endif