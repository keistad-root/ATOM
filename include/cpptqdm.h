#ifndef __CPPTQDM__
#define __CPPTQDM__

#include <chrono>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include <math.h>

class ProgressBar {
private:
    std::chrono::system_clock::time_point start_time;
    int mTerminalWidth;
    int mSetSize;
    std::chrono::system_clock::time_point printPoint;
    int called = 0;


public:
    ProgressBar();
    ProgressBar(int setSize);
    ~ProgressBar();

    void getTerminalLength();

    void printProgress();
    int getSecond(int num);
    int getMinute(int num);
};


#endif
