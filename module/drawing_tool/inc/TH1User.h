#ifndef __TH1USER__
#define __TH1USER__

#include "TH1.h"
#include "TSharedConfig.h"
#include "CppConfigFile.h"

class TH1User : public TH1 {
private:
	CppConfigDictionary mConfig;
public:
	TH1User(const CppConfigDictionary& config);
private:
	TSharedConfig sConfig;
};


#endif