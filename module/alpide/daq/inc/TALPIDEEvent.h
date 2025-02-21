#ifndef __TALPIDEEVENT__
#define __TALPIDEEVENT__

#ifdef __TALPIDEEVENT_HEADER__
#include <unordered_set>
#endif

#include <utility>
#include <vector>
#include<algorithm>

#include "TEvent.h"

class TALPIDEEvent : public TEvent {
public:
	TALPIDEEvent();

private:
	long int iTime;
	std::vector<std::pair<int, int>> data;

public:
	// Setter
	void setTime(const long int time);

	// Getter
	long int getTime() const;
	const std::vector<std::pair<int, int>>& getData() const;

	void removePixel(const std::pair<int, int>& coordinate);
	void pushData(const std::pair<int, int>& coordinate);
	void removeDuplication();
	void sortPixel();

	int getNData() const;

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