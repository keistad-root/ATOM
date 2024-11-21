#include "TAPTSEvent.h"

TAPTSEvent::TAPTSEvent() : TEvent() { }

void TAPTSEvent::setFrame(int frame) {
	iFrame = frame;
}

int TAPTSEvent::getFrame() {
	return iFrame;
}

std::array<int, 16>& TAPTSEvent::getData() {
	return data;
}