#ifndef __TALPIDE__
#define __TALPIDE__

#include <iostream>
#include <array>
#include <vector>

class TALPIDE {
private:
    int iEvent;
    long int iTime;
    std::vector<std::array<int,2>> data;
public:
    void setEvent(int event);
    void setTime(long int time);
    void pushData(std::array<int,2> coordinate);
    int getEvent();
    long int getTime();
    std::vector<std::array<int,2>>& getData();
};

#endif