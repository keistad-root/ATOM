#ifndef __EVENT__
#define __EVENT__

#include <vector>

#include "TROOT.h"

#include "frame.h"

class Event {
private:
    Int_t nframe;
    std::vector<Frame> event;
public:
    Event();
};

#endif