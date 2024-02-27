#include "TDecoder.h"

TDecoder::TDecoder(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "File " << path << " cannot be found." << std::endl;
    }
    binaryFile_ = std::ifstream(path,std::ios::binary);
}

TDecoder::TDecoder(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "File " << path << " cannot be found." << std::endl;
    }
    binaryFile_ = std::ifstream(path,std::ios::binary);
}

void TDecoder::readFile() {
    char buf[sizeof(uint8_t)];
    while (binaryFile_.read(buf, sizeof(buf))) {
        binaryData_.push_back(static_cast<uint8_t>(*buf));
    }
    dataLength_ = binaryData_.size();
}

int TDecoder::getDataLength() {
    return dataLength_;
}

std::vector<uint8_t>& TDecoder::getBinaryData() {
    return binaryData_;
}