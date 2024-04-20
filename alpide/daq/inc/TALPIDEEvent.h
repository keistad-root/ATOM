#ifndef __TALPIDEEVENT__
#define __TALPIDEEVENT__

#ifdef __TALPIDEEVENT_HEADER__
#include <unordered_set>
#endif

#include <utility>
#include <vector>

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

private:
	unsigned int fBits;
public:
	enum {
		kNotDeleted = 0x02000000
	};
	bool IsDestructed() const { return !TestBit(kNotDeleted); }
	bool TestBit(unsigned int f) const { return (bool) ((fBits & f) != 0); }
};

#endif