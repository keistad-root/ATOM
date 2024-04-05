#ifndef __TTHRESHOLD__
#define __TTHRESHOLD__

#include "Headers.h"

class TThreshold {
private:
    int mX;
    int mY;
    std::array<Int_t,50> mDacs;
    double mThr;
    double mErr;
    double mQualityFactor;

    std::unique_ptr<TGraph> thresholdGraph;
    std::unique_ptr<TF1> fitFunction;
public:
    //Constructor
    TThreshold();
    TThreshold(int x, int y);
    TThreshold(const std::array<int,2>& coordinate);
    TThreshold(int x, int y, const std::array<Int_t, 50>& dacs);
    TThreshold(const std::array<int,2>& coodrdinate, const std::array<Int_t, 50>& dacs);
    TThreshold(int x, int y, std::array<Int_t, 50>&& dacs);
    TThreshold(const std::array<int,2>& coodrdinate, std::array<Int_t, 50>&& dacs);
    //Copy Constructor
    TThreshold(const TThreshold& copy);
    //Copy Assignment
    TThreshold& operator=(const TThreshold& copy);
    //Move Constructor
    TThreshold(TThreshold&& move);
    //Move Assignment
    TThreshold& operator=(TThreshold&& move);
    //Destructor
    ~TThreshold();

    void calculateThreshold();
    void savePlot();

    const double getX() const;
    const double getY() const;
    const double getThreshold() const;
    const double getError() const;
    const double getQualityFactor() const;
};

#endif