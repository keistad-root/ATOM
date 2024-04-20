#ifndef __TAPTSDECODER__
#define __TAPTSDECODER__

#ifdef __TAPTSDECODER_HEADER__
#include "TAPTSEvent.h"
#endif

#include<vector>
#include<array>
#include<string>

#include "TDecoder.h"

class TAPTSEvent;

class TAPTSDecoder : public TDecoder {
private:
	std::vector<TAPTSEvent*> aptss;
	bool mux_ = false;
	int iEvent_ = 0;
	std::array<int, 16> mapping;
	std::array<uint8_t, 4> expected_header = {0xAA, 0xAA, 0xAA, 0xAA};
	std::array<uint8_t, 4> expected_footer = {0xBB, 0xBB, 0xBB, 0xBB};

	int nFrame_;
	int index_ = 0;
	int lenEvent_ = 0;

public:
	TAPTSDecoder(const std::filesystem::path& binaryPath);
	TAPTSDecoder(const std::string& binaryPath);
	void decode();
	std::vector<TAPTSEvent*> getData();
private:
	void inputEvent();
	void preTest(); // do range, header, missing test and calculate data length of Event.
	void rangeTest(); // Test whether the index number is overflowed or not.
	void headerTest(); // Test whether an event is normal or not. The first 4 num should be 0xAA.
	void missingTest(); // Test whether the data is overflowed or not.
	int getEventLength(); // Calculate the data length of an event.
	void postTest();
	bool isDone();
};
#endif