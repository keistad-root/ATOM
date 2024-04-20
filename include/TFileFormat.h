#ifndef __TFILEFORMAT__
#define __TFILEFORMAT__

#include "TTree.h"
#include "TH2.h"
#include "TH1.h"

struct TInputRoot {
	UChar_t chipid;
	UInt_t timeStamp;
	UShort_t x;
	UShort_t y;
};

#endif