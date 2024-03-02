#ifndef __TCHIP__
#define __TCHIP__

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "cppconfig.h"


class TChip {
private:
    std::string name_;
    std::vector<std::array<int,2>> mask;
    float materialBudget_;
    std::array<float,3> orientation_;
    std::array<float,2> pixelPitch_;
    std::array<float,3> position_;
    std::array<std::array<int,2>,4> regionOfInterest_;
    std::array<float,2> spatialResolution_;
    float timeResolution_;
public:
    TChip();
    TChip(Configurable iniConfig);
    void setName(const std::string& name);
    void setNoise(const std::string& fileName);
    void setMP(const std::string& materialBudget);
    void setOrientation(const std::string& orientation);
    void setPixelPitch(const std::string& pixelPitch);
    void setPosition(const std::string& position);
    void setROI(const std::string& regionOfInterest);
    void setSpatialResolution(const std::string& spatialResolution);
    void setTimeResolution(const std::string& timeResolution);
};

class TALPIDE : public TChip{
private:
    int iEvent;
    long int iTime;
    std::vector<std::array<int,2>> data;
public:
    TALPIDE();
    TALPIDE(Configurable iniConfig);
    void setEvent(int event);
    void setTime(long int time);
    void pushData(std::array<int,2> coordinate);
    int getEvent();
    long int getTime();
    std::vector<std::array<int,2>>& getData();
};

class TAPTS : public TChip {
private:
    int iEvent;
    int iFrame;
    std::array<int,16> data{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
public:
    TAPTS();
    TAPTS(Configurable iniConfig);
    void setEvent(int event);
    void setFrame(int frame);
    int getEvent();
    int getFrame();
    std::array<int,16>& getData();
};



#endif