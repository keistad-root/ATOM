#include "TH1User.h"

TH1User::TH1User(const CppConfigDictionary& config) : mConfig(config), TH1() {
	TString titleStr = config.hasKey("title") ? config.find("title") : " ";
	SetTitle(titleStr);
	sConfig.title = titleStr;

	// setTitle(titleStr);
}


