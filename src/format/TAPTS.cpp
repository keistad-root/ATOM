#include "TAPTS.h"

TAPTS::TAPTS() {}

TAPTS::TAPTS(Configurable iniConfig) : TChip(iniConfig) {}

void TAPTS::setEvent(int event) {
    iEvent = event;
}

void TAPTS::setFrame(int frame) {
    iFrame = frame;
}

int TAPTS::getEvent() {
    return iEvent;
}

int TAPTS::getFrame() {
    return iFrame;
}

std::array<int,16>& TAPTS::getData() {
    return data;
}