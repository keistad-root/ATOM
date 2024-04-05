#include "GarfieldParticle.h"

GarfieldParticle::GarfieldParticle(std::string particleName, double ekin_eV, double time, double x_cm, double y_cm, double z_cm, double dx, double dy, double dz) : mParticleName(particleName), mEkin_MeV(ekin_eV / 1000000), mTime(time), mX_mm(x_cm * 10), mY_mm(y_cm * 10), mZ_mm(z_cm * 10), mDx(dx), mDy(dy), mDz(dz) {}

GarfieldParticle::~GarfieldParticle() {}

std::string GarfieldParticle::getParticleName() const { return mParticleName; }

double GarfieldParticle::getX_mm() const { return mX_mm; }

double GarfieldParticle::getY_mm() const { return mY_mm; }

double GarfieldParticle::getZ_mm() const { return mZ_mm; }

double GarfieldParticle::getEkin_MeV() const { return mEkin_MeV; }

double GarfieldParticle::getTime() const { return mTime; } 

double GarfieldParticle::getDX() const { return mDx; }

double GarfieldParticle::getDY() const { return mDy; }

double GarfieldParticle::getDZ() const { return mDz; }