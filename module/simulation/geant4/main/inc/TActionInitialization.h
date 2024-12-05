#ifndef __TActionInitialization__
#define __TActionInitialization__

#include "G4VUserActionInitialization.hh"

#include "TPrimaryGeneratorAction.h"

#include "TRunAction.h"
#include "TEventAction.h"
#include "TSteppingAction.h"
#include "TTrackingAction.h"

class TActionInitialization : public G4VUserActionInitialization {

public:
	TActionInitialization() = default;
	~TActionInitialization() override = default;

	void BuildForMaster() const override;
	void Build() const override;
};

#endif