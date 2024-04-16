#ifndef __TAPTSEVENT__
#define __TAPTSEVENT__

#include "TEvent.h"

class TAPTSEvent : public TEvent {
private:
	int iFrame;
	std::array<int, 16> data{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

public:
	TAPTSEvent();

	// Setter
	void setFrame(int frame);

	// Getter
	int getFrame();
	std::array<int, 16>& getData();
};

#endif