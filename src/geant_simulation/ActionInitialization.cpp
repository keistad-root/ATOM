#include "ActionInitialization.h"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {
    G4cout << "\033[1;35m" << "ActionInitializer" << "\033[0m" << " is armed" << G4endl;
}

ActionInitialization::~ActionInitialization() {
    G4cout << "\033[1;35m" << "ActionInitializer" << "\033[0m" << " is terminated" << G4endl;
}

void ActionInitialization::Build() const {
    G4cout << "\033[1;35m" << "ActionInitializer" << "\033[0m" << " boots" << "\033[1;36m" << " Primary Generator Action" << "\033[0m" << G4endl;
    SetUserAction(new PrimaryGeneratorAction);
    G4cout << "\033[1;35m" << "ActionInitializer" << "\033[0m" << " boots" << "\033[1;36m" << " Run Action" << "\033[0m" << G4endl;
    RunAction* runAction = new RunAction;
    SetUserAction(runAction);

    G4cout << "\033[1;35m" << "ActionInitializer" << "\033[0m" << " boots" << "\033[1;36m" << " Event Action" << "\033[0m" << G4endl;
    EventAction* eventAction = new EventAction(runAction);
    SetUserAction(eventAction);

    G4cout << "\033[1;35m" << "ActionInitializer" << "\033[0m" << " boots" << "\033[1;36m" << " Stepping Action" << "\033[0m" << G4endl;
    SetUserAction(new SteppingAction(eventAction));

    G4cout << "\033[1;35m" << "ActionInitializer" << "\033[0m" << " boots" << "\033[1;36m" << " Tracking Action" << "\033[0m" << G4endl;
    SetUserAction(new TrackingAction);
}

void ActionInitialization::BuildForMaster() const {
    G4cout << "ActionInitialization::BuildForMaster()" << G4endl;
    RunAction* runAction = new RunAction;
    SetUserAction(runAction);
}
