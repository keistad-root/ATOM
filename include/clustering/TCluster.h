#ifndef __TCLUSTER__
#define __TCLUSTER__

#include <vector>
#include <array>

class TCluster {
private:
    int nEvent;
    int timeStamp;
    std::vector<std::array<int,2>> pixels;
    int minX = 1024, minY = 512;
    int maxX = 0, maxY = 0;
public:
    TCluster();
    TCluster(int event, int time);
    TCluster(const TCluster& copy);
    void AddPixel(std::array<int,2> pixel);
    void AddCluster(TCluster& cluster);
    std::vector<std::array<int,2>> getPixels();
    bool isNeighbour(std::array<int,2> pixel);
    bool isNeighbour(TCluster& cluster);
    bool isContain(std::array<int,2> pixel);
    bool isContain(TCluster& cluster);

    int getDistance(std::array<int,2>& pixel1, std::array<int,2>& pixel2);

    int getEvent();
    int getTimeStamp();

    int getMinX();
    int getMinY();
    int getMaxX();
    int getMaxY();

    std::array<double,2> getCenter();
    int getClusterSize();
};


#endif