#ifndef __TFILEFORMAT__
#define __TFILEFORMAT__

typedef unsigned char UChar_t;
typedef unsigned int UInt_t;
typedef unsigned short UShort_t;

struct TInputRoot {
	UChar_t chipid;
	UInt_t timeStamp;
	UShort_t x;
	UShort_t y;
};

#endif