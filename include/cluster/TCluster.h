#ifndef __TCLUSTER__
#define __TCLUSTER__

#include "Headers.h"

class TCluster {
private:
    int mEvent;
    int mTime;
    std::vector<std::array<int,2>> mPixels;
    int mMinX = 1024, mMinY = 512;
    int mMaxX = 0, mMaxY = 0;
    
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
    void AddPixel(const std::array<int,2>& pixel);
    void AddCluster(const TCluster& cluster);
    
    // Compare functions
    bool isNeighbour(const std::array<int,2>& pixel) const;
    bool isNeighbour(const TCluster& cluster) const;
    bool isContain(const std::array<int,2>& pixel)const;
    bool isContain(const TCluster& cluster) const;

    // Getter
    const int getDistance(const std::array<int,2>& pixel1, const std::array<int,2>& pixel2) const;
    const std::array<double,2> getCenter() const;
    const int getClusterSize() const;
    const std::unique_ptr<TH2I> getShape() const;

    // Setter for member
    void setEvent(const int event);
    void setTimeStamp(const int time);
    void setMinX(const int minX);
    void setMinY(const int minY);
    void setMaxX(const int maxX);
    void setMaxY(const int maxY);

    // Getter for member
    const std::vector<std::array<int,2>> getPixels() const;
    const int getEvent() const;
    const int getTimeStamp() const;
    const int getMinX() const;
    const int getMinY() const;
    const int getMaxX() const;
    const int getMaxY() const;
};


#endif