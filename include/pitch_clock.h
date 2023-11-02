#ifndef __PITCH_CLOCK__
#define __PITCH_CLOCK__

#include <iostream>
#include <ctime>

class TClock {
private:
    clock_t start, finish;
    double duration;
public:
    TClock();
    void Measure();
    void EndProgram();
};

#endif