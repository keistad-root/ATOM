#ifndef __ALPIDE_DECODER_H__
#define __ALPIDE_DECODER_H__

#include <iostream>

namespace ALPIDE {

const std::array<uint8_t,16> stray_sequence{0xaa,0xaa,0xaa,0xaa,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    
class TEvent {
private:
    int numEven_;
    long int time_;
    std::vector<std::array<int,2>> coordination_;
public:
    TEvent(const int& numEvent, const long int& time);
    void push_coordinate(const int& x, const int& y);
    int getEventNum() const;
    long int getTime() const;
    std::vector<std::array<int,2>> getCoords() const;
};

class TDecoder {
    
public:
    TDecoder();
    TEvent& alpide_decode_event(const std::vector<uint8_t>& data, int& i);
    bool stray_test(const std::vector<uint8_t>& data);

    void header_test(const std::vector<uint8_t>& data);
    long int hex_to_dec(const uint8_t* data, const int& digits);
    
    int bitwise_and(int v1, int v2);
    int bitwise_or(int v1, int v2);
    int bitwise_xor(int v1, int v2);


};


}
#endif