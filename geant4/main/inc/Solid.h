#ifndef __SOLID__
#define __SOLID__

#ifdef __SOLID_HEADER__
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4SystemOfUnits.hh"
#endif

class G4VSolid;


class Solid {
private:
	G4VSolid* alphaStandSolid;
	G4VSolid* betaStandSolid;
	G4VSolid* newStandSolid;
	G4VSolid* screenSolid;
	G4VSolid* alpideCircuitSolid;
	G4VSolid* alpideEpitaxialSolid;
	G4VSolid* boardSolid;
public:
	Solid();

	void setAlphaStandSolid();
	void setBetaStandSolid();
	void setNewStandSolid(double diameter);
	void setScreenSolid();
	void setAlpideCircuitSolid();
	void setAlpideEpitaxialSolid();
	void setBoardSolid();

	G4VSolid* getAlphaStandSolid() const;
	G4VSolid* getBetaStandSolid() const;
	G4VSolid* getNewStandSolid() const;
	G4VSolid* getScreenSolid() const;
	G4VSolid* getAlpideCircuitSolid() const;
	G4VSolid* getAlpideEpitaxialSolid() const;
	G4VSolid* getBoardSolid() const;
};

#endif