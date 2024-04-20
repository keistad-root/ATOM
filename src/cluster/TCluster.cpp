#include "TCluster.h"

TCluster::TCluster() : fBits(kNotDeleted) { };

TCluster::TCluster(int event, int time) : mEvent(event), mTime(time), fBits(kNotDeleted) { }

TCluster::TCluster(const TCluster& copy) : mEvent(copy.mEvent), mTime(copy.mTime), mMinX(copy.mMinX), mMinY(copy.mMinY), mMaxX(copy.mMaxX), mMaxY(copy.mMaxY), fBits(kNotDeleted) {
	mPixels.assign(copy.mPixels.begin(), copy.mPixels.end());
}

TCluster& TCluster::operator=(const TCluster& copy) {
	mEvent = copy.mEvent;
	mTime = copy.mTime;
	mPixels.assign(copy.mPixels.begin(), copy.mPixels.end());
	mMinX = copy.mMinX;
	mMinY = copy.mMinY;
	mMaxX = copy.mMaxX;
	mMaxY = copy.mMaxY;

	return *this;
}

TCluster::TCluster(TCluster&& move) : mEvent(move.mEvent), mTime(move.mTime), mPixels(std::move(move.mPixels)), mMinX(move.mMinX), mMinY(move.mMinY), mMaxX(move.mMaxX), mMaxY(move.mMaxY) { }

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

TCluster::~TCluster() { }

void TCluster::AddPixel(const std::pair<int, int>& pixel) {
	mPixels.push_back(pixel);
	mMaxY = std::max(mMaxY, pixel.second);
}

void TCluster::AddCluster(const TCluster& cluster) {
	for ( const std::pair<int, int>& pixel : cluster.getPixels() ) {
		mPixels.push_back(pixel);
	}
	mMaxY = std::max(mMaxY, cluster.getMaxY());
}

bool TCluster::isNeighbour(const std::pair<int, int>& pixel) const {
	bool neighbour = false;
	for ( std::pair<int, int> mPixel : mPixels ) {
		if ( getDistance(pixel, mPixel) <= 1 ) {
			neighbour = true;
			break;
		}
	}
	return neighbour;
}

bool TCluster::isNeighbour(const TCluster& cluster) const {
	bool neighbour = false;
	for ( std::pair<int, int> comparePixel : cluster.getPixels() ) {
		neighbour = isNeighbour(comparePixel);
		if ( neighbour ) break;
	}
	return neighbour;
}

bool TCluster::isContain(const std::pair<int, int>& pixel) const {
	bool neighbour = false;
	if ( find(mPixels.begin(), mPixels.end(), pixel) != mPixels.end() ) {
		neighbour = true;
	}
	return neighbour;
}

bool TCluster::isContain(const TCluster& cluster) const {
	bool neighbour = false;
	for ( std::pair<int, int> comparePixel : cluster.getPixels() ) {
		neighbour = isContain(comparePixel);
		if ( neighbour ) break;
	}
	return neighbour;
}

const int TCluster::getDistance(const std::pair<int, int>& pixel1, const std::pair<int, int>& pixel2) const {
	return abs(pixel1.first - pixel2.first) + abs(pixel1.second - pixel2.second);
}

void TCluster::calMembers() {
	calMinMax();
	calCenter();
	calSize();
	// calShape();
	// calStdevInAxis();
	// calStdev();
}

void TCluster::calMinMax() {
	for ( std::pair<int, int> pixel : mPixels ) {
		mMinX = std::min(pixel.first, mMinX);
		mMinY = std::min(pixel.second, mMinY);
		mMaxX = std::max(pixel.first, mMaxX);
		mMaxY = std::max(pixel.second, mMaxY);
	}
}

void TCluster::calCenter() {
	double x = 0., y = 0.;
	for ( std::pair<int, int> pixel : mPixels ) {
		x += pixel.first;
		y += pixel.second;
	}
	x = (double) x / mPixels.size();
	y = (double) y / mPixels.size();
	center = {x, y};
}

void TCluster::calSize() {
	size = mPixels.size();
}

const std::pair<double, double> TCluster::getCenter() const {
	return center;
}

const int TCluster::getSize() const {
	return size;
}

// Setter for member
void TCluster::setEvent(const int event) { mEvent = event; }
void TCluster::setTimeStamp(const int time) { mTime = time; }
void TCluster::setMinX(const int minX) { mMinX = minX; }
void TCluster::setMinY(const int minY) { mMinY = minY; }
void TCluster::setMaxX(const int maxX) { mMaxX = maxX; }
void TCluster::setMaxY(const int maxY) { mMaxY = maxY; }

// Getter for member
const std::vector <std::pair<int, int>> TCluster::getPixels() const { return mPixels; }
const int TCluster::getEvent() const { return mEvent; }
const int TCluster::getTimeStamp() const { return mTime; }
const int TCluster::getMinX() const { return mMinX; }
const int TCluster::getMinY() const { return mMinY; }
const int TCluster::getMaxX() const { return mMaxX; }
const int TCluster::getMaxY() const { return mMaxY; }

