#ifndef __TAPTS__
#define __TAPTS__

#include <iostream>
#include <array>
#include <vector>

class TAPTS {
private:
    int iEvent;
    int iFrame;
    std::array<int,16> data{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
public:
    void setEvent(int event);
    void setFrame(int frame);
    int getEvent();
    int getFrame();
    std::array<int,16>& getData();
};

#endif