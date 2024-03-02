#include "TChip.h"

TChip::TChip() {}

TChip::TChip(Configurable iniConfig) {
    setName(iniConfig.getConfigName());
    if (iniConfig.find("mask_file") == "") {
    } else {
        setNoise(iniConfig.find("mask_file"));
    }
    setMP(iniConfig.find("material_budget"));
    setOrientation(iniConfig.find("orientation"));

}

void TChip::setName(const std::string& name) {
    name_ = name;
}

void TChip::setNoise(const std::string& fileName) {
    std::string line;
    std::ifstream noiseFile(fileName);
    int x;
    int y;
    while (getline(noiseFile,line)) {
        x = stoi(line.substr(0,line.find(" ")));
        y = stoi(line.substr(line.find(" ")+1));
        mask.push_back({x,y});
    }
}

void TChip::setMP(const std::string& materialBudget) {
    materialBudget_ = stof(materialBudget);
}

void TChip::setOrientation(const std::string& orientation) {
    //0deg, 0deg, 0deg
    float x_axis, y_axis, z_axis;
    std::string temp;
    if (orientation.find("deg") != std::string::npos) {
        x_axis = stof(orientation.substr(0,orientation.find("deg")));
        temp = orientation.substr(5);
        std::cout << temp << std::endl;
        y_axis = stof(temp.substr(0,temp.find("deg")));
        temp = temp.substr(5);
        std::cout << temp << std::endl;
        z_axis = stof(temp.substr(0,temp.find("deg")));
    }
    orientation_ = {x_axis, y_axis, z_axis};
}
void TChip::setPixelPitch(const std::string& pixelPitch) {
}

void TChip::setTimeResolution(const std::string& timeResolution) {
    timeResolution_ = stof(timeResolution);
}