/**
 * @file TCluster.h
 * @author Yongjun Choi (yochoi@cern.ch)
 * @brief The TCluster class header. Cluster properties are defined.
 * @version 0.1
 * @date 2024-04-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __TCLUSTER__
#define __TCLUSTER__

#include "Headers.h"
#include "TMatrix2D.h"

class TCluster {
private:
	int mEvent;
	int mTime;
	std::vector <std::pair<int, int>> mPixels;

	int mMinX = 1024, mMinY = 512;
	int mMaxX = 0, mMaxY = 0;
	std::pair<double, double> center;
	std::pair<double, double> stdevInAxis;
	double stdev;
	int size = 0;
	TMatrix2D<int> shape;

public:
	// Constructor
	TCluster();
	TCluster(int event, int time);
	// Copy Constructor
	TCluster(const TCluster& copy);
	// Copy Assignment
	TCluster& operator=(const TCluster& copy);
	// Move Constructor
	TCluster(TCluster&& move);
	// Move Assignment
	TCluster& operator=(TCluster&& move);
	// Destructor
	~TCluster();

	// Add Function
	void AddPixel(const std::pair<int, int>& pixel);
	void AddCluster(const TCluster& cluster);

	// Compare functions
	bool isNeighbour(const std::pair<int, int>& pixel) const;
	bool isNeighbour(const TCluster& cluster) const;
	bool isContain(const std::pair<int, int>& pixel)const;
	bool isContain(const TCluster& cluster) const;
	const int getDistance(const std::pair<int, int>& pixel1, const std::pair<int, int>& pixel2) const;

	void calMembers();
	void calMinMax();
	void calCenter();
	void calStdevInAxis();
	void calStdev();
	void calSize();
	void calShape();

	// Getter
	const std::pair<double, double> getCenter() const;
	const std::pair<double, double> getStdevInAxis() const;
	const double getStdev() const;
	const int getSize() const;
	const TMatrix2D<int>& getShape() const;

	// Setter for member
	void setEvent(const int event);
	void setTimeStamp(const int time);
	void setMinX(const int minX);
	void setMinY(const int minY);
	void setMaxX(const int maxX);
	void setMaxY(const int maxY);

	// Getter for member
	const std::vector <std::pair<int, int>> getPixels() const;
	const int getEvent() const;
	const int getTimeStamp() const;
	const int getMinX() const;
	const int getMinY() const;
	const int getMaxX() const;
	const int getMaxY() const;

	bool operator==(const TCluster& cluster) const;
	bool operator!=(const TCluster& cluster) const;
};

#endif