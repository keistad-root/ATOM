#include "TALPIDE.h"

void TALPIDE::setEvent(int event) {
    iEvent = event;
}

void TALPIDE::setTime(long int time) {
    iTime = time;
}

void TALPIDE::pushData(std::array<int,2> coordinate) {
    data.push_back(coordinate);
}

int TALPIDE::getEvent() {
    return iEvent;
}

long int TALPIDE::getTime() {
    return iTime;
}

std::vector<std::array<int,2>>& TALPIDE::getData() {
    return data;
}