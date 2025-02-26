#ifndef __TCOLOURUSER__
#define __TCOLOURUSER__

#include<string>
#include<array>
#include "TColor.h"

class TColourUser {
public:
	static Int_t getColour(const std::string colorName);
};
#endif