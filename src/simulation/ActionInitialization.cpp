#include "ActionInitialization.h"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const {

    SetUserAction(new PrimaryGeneratorAction);
    RunAction* runAction = new RunAction;
    SetUserAction(runAction);

    EventAction* eventAction = new EventAction(runAction);
    SetUserAction(eventAction);

    SetUserAction(new SteppingAction(eventAction));
    SetUserAction(new TrackingAction);
}

void ActionInitialization::BuildForMaster() const {
    G4cout << "ActionInitialization::BuildForMaster()" << G4endl;
    RunAction* runAction = new RunAction;
    SetUserAction(runAction);
}
