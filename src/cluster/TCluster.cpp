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
    mMaxY = std::max(mMaxY,pixel[1]);
}

void TCluster::AddCluster(const TCluster& cluster) {
    for (const std::array<int,2>& pixel : cluster.getPixels()) {
        mPixels.push_back(pixel);
    }
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

void TCluster::calMembers() {
    calMinMax();
    calCenter();
    calSize();
    calShape();
}

void TCluster::calMinMax() {
    for (std::array<int, 2> pixel : mPixels) {
        mMinX = std::min(pixel[0], mMinX);
        mMinY = std::min(pixel[1], mMinY);
        mMaxX = std::max(pixel[0], mMaxX);
        mMaxY = std::max(pixel[1], mMaxY);
    }
}

void TCluster::calCenter() {
    double x = 0., y = 0.;
    for (std::array<int, 2> pixel : mPixels) {
        x += pixel[0];
        y += pixel[1];
    }
    x = x / mPixels.size();
    y = y / mPixels.size();
    center = {x, y};
}

void TCluster::calSize() {
    size = mPixels.size();
}

void TCluster::calShape() {
    shape = TMatrix2D<int>(mMaxX - mMinX + 1, mMaxY - mMinY + 1);
    for (std::array<int, 2> pixel : mPixels) {
        shape.setElement(pixel[0] - mMinX, pixel[1] - mMinY, 1);
    }
}

const std::pair<double, double> TCluster::getCenter() const {
    return center;
}

const int TCluster::getSize() const { 
    return size; 
}

const TMatrix2D<int>& TCluster::getShape() const {
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

