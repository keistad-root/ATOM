#include "TCluster.h"

TCluster::TCluster() {};

TCluster::TCluster(int event, int time) : mEvent(event), mTime(time) {}

TCluster::TCluster(const TCluster& copy) : mEvent(copy.mEvent), mTime(copy.mTime), mMinX(copy.mMinX), mMinY(copy.mMinY), mMaxX(copy.mMaxX), mMaxY(copy.mMaxY) {
    mPixels.assign(copy.mPixels.begin(),copy.mPixels.end());
}

TCluster& TCluster::operator=(const TCluster& copy) {
    mEvent = copy.mEvent;
    mTime = copy.mTime;
    mPixels.assign(copy.mPixels.begin(),copy.mPixels.end());
    mMinX = copy.mMinX;
    mMinY = copy.mMinY;
    mMaxX = copy.mMaxX;
    mMaxY = copy.mMaxY;

    return *this;
}

TCluster::TCluster(TCluster&& move) : mEvent(move.mEvent), mTime(move.mTime), mPixels(std::move(move.mPixels)), mMinX(move.mMinX), mMinY(move.mMinY), mMaxX(move.mMaxX), mMaxY(move.mMaxY) {}

TCluster& TCluster::operator=(TCluster&& move) {
    mEvent = move.mEvent;
    mTime = move.mTime;
    mPixels = std::move(move.mPixels);
    mMinX = move.mMinX;
    mMinY = move.mMinY;
    mMaxX = move.mMaxX;
    mMaxY = move.mMaxY;
    return *this;
}

TCluster::~TCluster() {}

void TCluster::AddPixel(const std::array<int,2>& pixel) {
    mPixels.push_back(pixel);
    mMinX = std::min(mMinX,pixel[0]);
    mMinY = std::min(mMinY,pixel[1]);
    mMaxX = std::max(mMaxX,pixel[0]);
    mMaxY = std::max(mMaxY,pixel[1]);
}

void TCluster::AddCluster(const TCluster& cluster) {
    for (const std::array<int,2>& pixel : cluster.getPixels()) {
        mPixels.push_back(pixel);
    }
    mMinX = std::min(mMinX,cluster.getMinX());
    mMinY = std::min(mMinY,cluster.getMinY());
    mMaxX = std::max(mMaxX,cluster.getMaxX());
    mMaxY = std::max(mMaxY,cluster.getMaxY());
}

bool TCluster::isNeighbour(const std::array<int,2>& pixel) const {
    bool neighbour = false;
    for (std::array<int,2> mPixel : mPixels) {
        if (getDistance(pixel,mPixel) <=1) {
            neighbour = true;
            break;
        }
    }
    return neighbour;
}

bool TCluster::isNeighbour(const TCluster& cluster) const {
    bool neighbour = false;
    for (std::array<int,2> comparePixel : cluster.getPixels()) {
        neighbour = isNeighbour(comparePixel);
        if (neighbour) break;
    }
    return neighbour;
}

bool TCluster::isContain(const std::array<int,2>& pixel) const {
    bool neighbour = false;
    if (find(mPixels.begin(),mPixels.end(),pixel) != mPixels.end()) {
        neighbour = true;
    }
    return neighbour;
}

bool TCluster::isContain(const TCluster& cluster) const {
    bool neighbour = false;
    for (std::array<int,2> comparePixel : cluster.getPixels()) {
        neighbour = isContain(comparePixel);
        if (neighbour) break;
    }
    return neighbour;
}

const int TCluster::getDistance(const std::array<int,2>& pixel1, const std::array<int,2>& pixel2) const {
    return abs(pixel1[0]-pixel2[0]) + abs(pixel1[1]-pixel2[1]);
}

const std::array<double,2> TCluster::getCenter() const {
    double x = 0., y = 0.;
    for (std::array<int,2> pixel : mPixels) {
        x += pixel[0];
        y += pixel[1];
    }
    x = x / mPixels.size();
    y = y / mPixels.size();
    return {x, y};
}

const int TCluster::getClusterSize() const { 
    return mPixels.size(); 
}

const std::unique_ptr<TH2I> TCluster::getShape() const {
    std::unique_ptr<TH2I> shape(new TH2I("shape", "Cluster Shape; x; y", mMaxX-mMinX, -.5, mMaxX-mMinX -.5, mMaxY-mMinY, -.5, mMaxY-mMinY -.5));
    for (const std::array<int,2>& mPixel : mPixels) {
        shape->Fill(mPixel[0] - mMinX + 1, mPixel[1] - mMinY + 1);
    }
    return shape;
}

// Setter for member
void TCluster::setEvent(const int event) { mEvent = event; }
void TCluster::setTimeStamp(const int time) {mTime = time; }
void TCluster::setMinX(const int minX) { mMinX = minX; }
void TCluster::setMinY(const int minY) { mMinY = minY; }
void TCluster::setMaxX(const int maxX) { mMaxX = maxX; }
void TCluster::setMaxY(const int maxY) { mMaxY = maxY; }

// Getter for member
const std::vector<std::array<int,2>> TCluster::getPixels() const { return mPixels; }
const int TCluster::getEvent() const { return mEvent; }
const int TCluster::getTimeStamp() const { return mTime; }
const int TCluster::getMinX() const { return mMinX; }
const int TCluster::getMinY() const { return mMinY; }
const int TCluster::getMaxX() const { return mMaxX; }
const int TCluster::getMaxY() const { return mMaxY; }