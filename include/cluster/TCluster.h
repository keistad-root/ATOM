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
	int mEvent; /**< Number of event to which this cluster belongs */
	int mTime; /**< Time stamp of  event */
	std::vector <std::pair<int, int>> mPixels; /**< The bundle of pixels which are composed to this cluster */

	int mMinX = 1024, mMinY = 512; /**< Maximum pixel x and y value in cluster*/
	int mMaxX = 0, mMaxY = 0; /**< Minimum pixel x and y value in cluster*/
	std::pair<double, double> center; /**< cluster centre value. The average of x and y values.*/
	// std::pair<double, double> stdevInAxis; /**< standard deviation in x and y direction */
	// double stdev; /**< Root of mean of squared distance from cluster centre */
	int size = 0; /**< The number of pixels in cluster */
	// TMatrix2D<int> shape; /**< The shape matrix for analyse cluster shape */

public:
	TCluster();
	TCluster(int event, int time);
	TCluster(const TCluster& copy);
	TCluster& operator=(const TCluster& copy);
	TCluster(TCluster&& move);
	TCluster& operator=(TCluster&& move);
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
	// void calStdevInAxis();
	// void calStdev();
	void calSize();
	// void calShape();

	// Getter
	const std::pair<double, double> getCenter() const;
	// const std::pair<double, double> getStdevInAxis() const;
	// const double getStdev() const;
	const int getSize() const;
	// const TMatrix2D<int>& getShape() const;

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

private:
	uint fBits;
public:
	enum {
		kNotDeleted = 0x02000000
	};
	bool IsDestructed() const { return !TestBit(kNotDeleted); }
	bool TestBit(uint f) const { return (bool) ((fBits & f) != 0); }
};

#endif