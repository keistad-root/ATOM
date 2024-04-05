#ifndef __GARFIELDPARTICLE__
#define __GARFIELDPARTICLE__

#include <string>

class GarfieldParticle {
private:
    std::string mParticleName;
    double mEkin_MeV, mTime, mX_mm, mY_mm, mZ_mm, mDx, mDy, mDz;
public:
    GarfieldParticle(std::string particleName, double ekin_eV, double time, double x_cm, double y_cm, double z_cm, double dx, double dy, double dz);
    ~GarfieldParticle();

    std::string getParticleName() const;
    double getX_mm() const;
    double getY_mm() const;
    double getZ_mm() const;
    double getEkin_MeV() const;
    double getTime() const;
    double getDX() const;
    double getDY() const;
    double getDZ() const;
};

#endif