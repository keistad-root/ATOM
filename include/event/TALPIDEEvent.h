#ifndef __TALPIDEEVENT__
#define __TALPIDEEVENT__

#include "TEvent.h"

class TALPIDEEvent : public TEvent {
private:
	long int iTime;
	std::vector<std::pair<int, int>> data;

public:
	TALPIDEEvent();
	TALPIDEEvent(const TALPIDEEvent& copy);
	TALPIDEEvent& operator=(const TALPIDEEvent& copy);
	TALPIDEEvent(TALPIDEEvent&& move);
	TALPIDEEvent& operator=(TALPIDEEvent&& move);

	// Setter
	void setTime(const long int time);

	// Getter
	const long int getTime() const;
	const std::vector<std::pair<int, int>>& getData() const;

	void removePixel(const std::pair<int, int>& coordinate);
	void pushData(const std::pair<int, int>& coordinate);
	void removeDuplication();
	void sortPixel();
};

#endif