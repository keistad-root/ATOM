#include "GarfieldPhysics.h"

GarfieldPhysics::~GarfieldPhysics() {
    std::cout << "Deconstructor GarfieldPhysics" << std::endl;
}

GarfieldPhysics* GarfieldPhysics::GetInstance() {
    if (!mPhysics) mPhysics = new GarfieldPhysics();
    return mPhysics;
}

void GarfieldPhysics::Dispose() {
    delete mPhysics;
    mPhysics = nullptr;
}

void GarfieldPhysics::InitializationPhysics() { // Need More
    mMediumSilicon = new Garfield::MediumSilicon();
    mMediumSilicon->SetTemperature(293.15);
}

void GarfieldPhysics::DoIt(std::string particleName, double ekin_MeV, double time, double x_cm, double y_cm, double z_cm, double dx, double dy, double dz) {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    mEnergyDeposit = 0.;
    mSecondaryParticle.clear();

    Garfield::AvalancheMC drift;
    drift.SetSensor(mSensor);
    drift.SetDistanceSteps(1.e-4); // Step = um

    // Need to invest the neccesary of this object in case of silicon.
    Garfield::AvalancheMicroscopic avalanche;
    avalanche.SetSensor(mSensor);

    double eKin_eV = ekin_MeV * 1e+6;

    mEnergyDeposit = 0;

    int nc = 0;

    // fTrackHeed->TransprotDeltaE
}
