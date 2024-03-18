#include "TCluster.h"

TCluster::TCluster() {};

TCluster::TCluster(int event, int time) : nEvent(event), timeStamp(time) {}

TCluster::TCluster(const TCluster& copy) : nEvent(copy.nEvent), timeStamp(copy.timeStamp), minX(copy.minX), minY(copy.minY), maxX(copy.maxX), maxY(copy.maxY) {
    pixels.assign(copy.pixels.begin(),copy.pixels.end());
}

void TCluster::AddPixel(std::array<int,2> pixel) {
    pixels.push_back(pixel);
    minX = std::min(minX,pixel[0]);
    minY = std::min(minY,pixel[1]);
    maxX = std::max(maxX,pixel[0]);
    maxY = std::max(maxY,pixel[1]);
}

std::vector<std::array<int,2>> TCluster::getPixels() {
    return pixels;
}

void TCluster::AddCluster(TCluster& cluster) {
    for (std::array<int,2> pixel : cluster.getPixels()) {
        pixels.push_back(pixel);
    }
    minX = std::min(minX,cluster.getMinX());
    minY = std::min(minY,cluster.getMinY());
    maxX = std::max(maxX,cluster.getMaxX());
    maxY = std::max(maxY,cluster.getMaxY());
}

bool TCluster::isNeighbour(std::array<int,2> pixel) {
    bool neighbour = false;
    for (std::array<int,2> clusterPixel : pixels) {
        if (getDistance(pixel,clusterPixel) <=1) {
            neighbour = true;
            break;
        }
    }
    return neighbour;
}

bool TCluster::isNeighbour(TCluster& cluster) {
    bool neighbour = false;
    for (std::array<int,2> comparePixel : cluster.getPixels()) {
        neighbour = isNeighbour(comparePixel);
        if (neighbour) break;
    }
    return neighbour;
}

bool TCluster::isContain(std::array<int,2> pixel) {
    bool neighbour = false;
        if (find(pixels.begin(),pixels.end(),pixel) != pixels.end()) {
            neighbour = true;
        }
    return neighbour;
}

bool TCluster::isContain(TCluster& cluster) {
    bool neighbour = false;
    for (std::array<int,2> comparePixel : cluster.getPixels()) {
        neighbour = isContain(comparePixel);
        if (neighbour) break;
    }
    return neighbour;
}

int TCluster::getDistance(std::array<int,2>& pixel1, std::array<int,2>& pixel2) {
    return abs(pixel1[0]-pixel2[0]) + abs(pixel1[1]-pixel2[1]);
}

int TCluster::getEvent() {
    return nEvent;
}

int TCluster::getTimeStamp() {
    return timeStamp;
}

int TCluster::getMinX() {
    return minX;
}

int TCluster::getMinY() {
    return minY;
}

int TCluster::getMaxX() {
    return maxX;
}

int TCluster::getMaxY() {
    return maxY;
}

std::array<double,2> TCluster::getCenter() {
    double x = 0., y = 0.;
    for (std::array<int,2> pixel : pixels) {
        x += pixel[0];
        y += pixel[1];
    }
    x = x / pixels.size();
    y = y / pixels.size();
    return {x, y};
}

int TCluster::getClusterSize() {
    return pixels.size();
}