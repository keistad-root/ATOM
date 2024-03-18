#ifndef __TALPIDEDECODER__
#define __TALPIDEDECODER__

#include "TDecoder.h"
#include "TALPIDEEvent.h"
#include "constant.h"

class TALPIDEDecoder : public TDecoder {
private:
    std::vector<std::unique_ptr<TALPIDEEvent>> alpides;
    int index_ = 0;
    
public:
    TALPIDEDecoder(const std::filesystem::path& binaryPath);
    TALPIDEDecoder(const std::string& binaryPath);
    void decode();
    std::vector<std::unique_ptr<TALPIDEEvent>> getData();
private:
    void inputEvent();
    bool isDone();
    void preTest();
    bool strayTest();
    void headerTest();
    long int hex_to_dec(const uint8_t* data, const int& digits);
    int bitwise_and(int v1, int v2);
    int bitwise_or(int v1, int v2);
    int bitwise_xor(int v1, int v2);
};
#endif