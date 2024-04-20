#ifndef __TDECODER__
#define __TDECODER__

#ifdef __TDECODER__
#include<iostream>
#endif

#include<fstream>
#include<vector>
#include<filesystem>
#include<string>

class TDecoder {
private:
	std::ifstream binaryFile_;
	int dataLength_;
	std::vector<uint8_t> binaryData_;
public:
	//Constructor
	TDecoder(const std::filesystem::path& binaryPath);
	TDecoder(const std::string& binaryPath);

	void readFile();

	int getDataLength();
	std::vector<uint8_t>& getBinaryData();
};

#endif