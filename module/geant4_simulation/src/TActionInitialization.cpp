#include "TActionInitialization.h"

TActionInitialization::TActionInitialization(const CppConfigFile config) : G4VUserActionInitialization(), mConfig(config) { }

void TActionInitialization::BuildForMaster() const {
	// In this method, the user can specify the actions that are to be taken by the master thread.
	TRunAction* runAction = new TRunAction;
	SetUserAction(runAction);
}

void TActionInitialization::Build() const {
	// In this method, the user can specify the actions that are to be taken by the worker threads.
	TPrimaryGeneratorAction* primaryGeneratorAction = new TPrimaryGeneratorAction(mConfig);
	primaryGeneratorAction->setAlphaEnergy();
	SetUserAction(primaryGeneratorAction);


	TRunAction* runAction = new TRunAction;
	SetUserAction(runAction);

	// The run action is passed to the event action.
	TEventAction* eventAction = new TEventAction(runAction);
	SetUserAction(eventAction);

	// The event action is passed to the stepping action.
	SetUserAction(new TSteppingAction(eventAction));
	SetUserAction(new TTrackingAction);
}