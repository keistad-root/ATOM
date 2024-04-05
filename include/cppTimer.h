#ifndef __CPPTIMER__
#define __CPPTIMER__

#include <iostream>
#include <ctime>

class TTimer {
private:
    clock_t start, finish;
    double duration;
public:
    TTimer();
    void Measure();
    void EndProgram();
};

#endif