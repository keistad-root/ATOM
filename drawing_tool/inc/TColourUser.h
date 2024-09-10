#ifndef __TCOLOURUSER__
#define __TCOLOURUSER__

#include<string>
#include "TColor.h"

int getColour(const std::string colorName) {
	if ( colorName == "red" ) {
		return kRed;
	} else if ( colorName == "magenta" ) {
		return kMagenta;
	} else if ( colorName == "blue" ) {
		return kBlue;
	} else if ( colorName == "cyan" ) {
		return kCyan + 2;
	} else if ( colorName == "orange" ) {
		return kOrange;
	} else if ( colorName == "violet" ) {
		return kViolet;
	} else if ( colorName == "green" ) {
		return kGreen + 2;
	} else {
		return kWhite;
	}
}

#endif