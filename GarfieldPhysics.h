#ifndef __GARFIELDPHYSICS__
#define __GARFIELDPHYSICS__

#include <iostream>

#include "GarfieldParticle.h"
#include "AnalysisManager.h"

#include "Garfield/MediumSilicon.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/AvalancheMicroscopic.hh"

class GarfieldPhysics {
private:
    Garfield::MediumSilicon* mMediumSilicon = nullptr;
    Garfield::Sensor* mSensor = nullptr;

    std::string fIonizationModel = "PAIPhot";
    static GarfieldPhysics* mPhysics;

    double mEnergyDeposit = 0;
    std::vector<GarfieldParticle> mSecondaryParticle;

public:
    GarfieldPhysics() = default;
    ~GarfieldPhysics();
    
    static GarfieldPhysics* GetInstance();
    void Dispose();

    void InitializationPhysics();

    void DoIt(std::string particleName, double ekin_MeV, double time, double x_cm, double y_cm, double z_cm, double dx, double dy, double dz);


};

#endif