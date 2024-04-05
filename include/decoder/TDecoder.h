#ifndef __TDECODER__
#define __TDECODER__

#include "Headers.h"

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