#ifndef __DETECTORCONSTRUCTION__
#define __DETECTORCONSTRUCTION__

#ifdef __DETECTORCONSTRUCTION_HEADER__
#include "Material.h"
#include "Colour.h"
#include "Solid.h"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4AssemblyVolume.hh"
#include "G4UserLimits.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#endif

#include "G4VUserDetectorConstruction.hh"

class Material;
class Colour;
class Solid;
class G4AssemblyVolume;

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
	Material* material = nullptr;
	Colour* colour = nullptr;
	Solid* solids = nullptr;

	G4VPhysicalVolume* WorldPhysical = nullptr;
	G4VPhysicalVolume* StandPhysical = nullptr;
	G4VPhysicalVolume* ShieldPhysical = nullptr;
	G4VPhysicalVolume* CarrierBoardPhysical = nullptr;

	G4LogicalVolume* WorldLogical = nullptr;
	G4LogicalVolume* StandLogical = nullptr;
	G4LogicalVolume* ShieldLogical = nullptr;
	G4LogicalVolume* ALPIDECircuitLogical = nullptr;
	G4LogicalVolume* ALPIDEEpitaxialLogical = nullptr;
	G4AssemblyVolume* ALPIDEAssembly = nullptr;
	G4LogicalVolume* CarrierBoardLogical = nullptr;

	SourceType sourceType;
	StandType standType = StandType::none;
	G4double sEnergy = 5.4;
	G4double sDistance = 10.;
	G4double cDiameter = 1.;
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();
	virtual G4VPhysicalVolume* Construct();
	G4VPhysicalVolume* Construct(G4String standType, G4double distance);

	// Setter
	void SetWorld(G4double air_pressure);
	void SetStand(G4String standType, G4double hallDiameter = 0.);
	void SetShield(G4double shieldWidth);
	void SetALPIDE(G4String alpideType);
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