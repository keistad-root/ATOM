#include "TEvent.h"

TEvent::TEvent() { }

TEvent::~TEvent() { }

void TEvent::setEvent(const int event) {
	iEvent = event;
}

int TEvent::getEvent() const {
	return iEvent;
}