#ifndef __TPHYSICSLIST__
#define __TPHYSICSLIST__

#include "G4VModularPhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonElasticPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4eBremsstrahlung.hh"

#include "G4ProcessManager.hh"

#include "G4StepLimiterPhysics.hh"

#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmCalculator.hh"

class TPhysicsList : public G4VModularPhysicsList {
public:
	TPhysicsList();
private:
	void ConstructProcess() override {
		G4VModularPhysicsList::ConstructProcess();

		G4ProcessManager* pManager = G4Alpha::Alpha()->GetProcessManager();
		if ( pManager ) {
			G4eBremsstrahlung* bremsstrahlung = new G4eBremsstrahlung();
			pManager->AddDiscreteProcess(bremsstrahlung);
		}
	}

	void SetCuts() override;
};

#endif