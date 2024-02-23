#include "alpide_decoder.h"

ALPIDE::TEvent::TEvent(const int& numEvent, const long int& time) 
: numEven_(numEvent), time_(time) 
{}

void ALPIDE::TEvent::push_coordinate(const int& x, const int& y) {
    std::array<int,2> xy;
        xy[0] = x;
        xy[1] = y;
        coordination_.push_back(xy);
}

int ALPIDE::TEvent::getEventNum() const {
    return numEven_;
}

long int ALPIDE::TEvent::getTime() const {
    return time_;
}

std::vector<std::array<int,2>> ALPIDE::TEvent::getCoords() const { return coordination_; }

ALPIDE::TDecoder::TDecoder() {}

ALPIDE::TEvent& ALPIDE::TDecoder::alpide_decode_event(const std::vector<uint8_t>& data, int& i) {
    if (i == 0) {
        if (stray_test(data)) i += 16;
        else ;
    }
    header_test(data); // If the head of data isn't AA AA AA AA, it throw an error.

    int numEvent = hex_to_dec(&*data.begin()+i+4,4);
    long int time = hex_to_dec(&*data.begin()+i+8,8);
    TEvent* event = new TEvent(numEvent,time);
    i += 4*4;

    if ( bitwise_and(data[i],0xF0) == 0xE0) { // chip has empty frame
        if ((data[i+2] != 0xFF) || data[i+3] != 0xFF) {
            std::cerr << "Assertion Error at empty frame test" << std::endl;
            throw std::bad_exception();
        }
        i += 4;
    } else if ( bitwise_and(data[i],0xF0) == 0xA0 ) {
        i += 2;
        int n = data.size();
        int reg = 0;
        while ( i < n ) {
            uint8_t data0 = data[i];
            if (bitwise_and(data0,0xC0) == 0x00) {  // data  long
                int d = bitwise_or(bitwise_or((reg * pow(2,14)), bitwise_and(data0,0x3F) * pow (2,8)), data[i+1]);
                int x = bitwise_or(bitwise_and((int) (d / pow(2,9)),0x3FE), bitwise_and(bitwise_xor(d,(int) (d / pow(2,1))),0x1));
                int y = bitwise_and((int) d / pow(2,1), 0x1FF);
                event->push_coordinate(x,y);
                uint8_t data2 = data[i+2];
                d+=1;
                while (data2) {
                    if (data2&1) {
                        int x = bitwise_or(bitwise_and((int) (d / pow(2,9)),0x3FE), bitwise_and(bitwise_xor(d,(int) (d / pow(2,1))),0x1));
                        int y = bitwise_and((int) d / pow(2,1), 0x1FF);
                        event->push_coordinate(x,y);
                    }
                    data2 = (int) (data2 / pow(2,1));
                    d+=1;
                }
                i+=3;
            } else if (bitwise_and(data0,0xC0) == 0x40) {
                int d = bitwise_or(bitwise_or((reg * pow(2,14)), bitwise_and(data0,0x3F) * pow (2,8)), data[i+1]);
                int x = bitwise_or(bitwise_and((int) (d / pow(2,9)),0x3FE), bitwise_and(bitwise_xor(d,(int) (d / pow(2,1))),0x1));
                int y = bitwise_and((int) d / pow(2,1), 0x1FF);
                event->push_coordinate(x,y);
                i+=2;
            } else if (bitwise_and(data0,0xE0) == 0xC0) {
                reg = bitwise_and(data0,0x1F);
                i += 1;
            } else if (bitwise_and(data0,0xF0) == 0xB0) {
                i += 1;
                i = static_cast<int> ((i+3) / 4*4);
                break;
            } else if (data0 == 0xFF) {
                i += 1;
            } else {
                std::cout << "DEBUG: data[i-10,i+10]:";
                for (int j = 0; j < 20; j++) {
                    std::cout << data[i+j-10];
                }
                std::cout << "ValueError: invalid literal" << std::endl;
                throw std::bad_exception();
            }
        }
    }
    i += 4;
    return *event;
}

bool ALPIDE::TDecoder::stray_test(const std::vector<uint8_t>& data) {
    if (std::equal(data.begin(),data.begin()+16,stray_sequence.begin())) {
        std::cout << "Decoder: stray sequence 0x AA AA AA AA 00 00 00 00 02 00 00 00 00 00 00 00 found. Skipping 16 bytes.";
        return true;
    } else {
        return false;
    }
}

void ALPIDE::TDecoder::header_test(const std::vector<uint8_t>& data) {
    if (!std::equal(data.begin(),data.begin()+4,stray_sequence.begin())) {
        std::cerr << "Assertion Error in header, not 0x AA AA AA AA" << std::endl;
        throw std::bad_exception();
    }
}

long int ALPIDE::TDecoder::hex_to_dec(const uint8_t* data, const int& digits) {
    long int val = 0;
    for (int digit = 0; digit < digits; digit++) {
        val += data[digit]*pow(256,digit);
    }
    return val;
}

int ALPIDE::TDecoder::bitwise_and(int v1, int v2) {
    int result = 0;
    uint8_t digit = 0;
    while(true) {
        result += (v1 % 2 * v2 % 2)*pow(2,digit);
        v1 = v1 / 2;
        v2 = v2 / 2;
        digit++;
        if (v1 == 0 || v2 == 0) break;
    }
    return result;
}

int ALPIDE::TDecoder::bitwise_or(int v1, int v2) {
    int result = 0;
    uint8_t digit = 0;
    while(true) {
        result -= ((v1 % 2 - 1) * (v2 % 2 - 1) - 1)*pow(2,digit);
        v1 = v1 / 2;
        v2 = v2 / 2;
        digit++;
        if (v1 == 0 && v2 == 0) break;
    }
    return result;
}

int ALPIDE::TDecoder::bitwise_xor(int v1, int v2) {
    int result = 0;
    uint8_t digit = 0;
    while(true) {
        result += (v1 % 2 + v2 % 2) % 2 * pow(2,digit);
        v1 = v1 / 2;
        v2 = v2 / 2;
        digit++;
        if (v1 == 0 && v2 == 0) break;
    }
    return result;
}