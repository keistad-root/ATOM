#include "Decoder.h"

TDecoder::TDecoder(std::string file_in, Bool_t mux) : i{0}, iev{0}, _mux{mux} {
    rawFile.open(file_in,std::ios::binary);
    int nev = 0;
    char buf[sizeof(uint8_t)];
    std::vector<uint8_t> values;
    while (rawFile.read(buf, sizeof(buf)))
    {
        values.push_back(0);
        memcpy(&values[nev], buf, sizeof(values[nev]));
        nev++;
    }
    data = new uint8_t[values.size()];
    std::copy(values.begin(), values.end(), data);

    _len_data=values.size();

    values.clear();
    std::vector<uint8_t>().swap(values);
}

TDecoder::~TDecoder() {
    delete data;
}

Bool_t TDecoder::is_done() {
    if (i >= _len_data) return kTRUE;
    else return kFALSE;
}

Int_t** TDecoder::get_next_event() {
    if (!(i+8<=_len_data)) {
        std::cout << "Ev " << iev << ": Unexpected data length (" << _len_data-1 << " < 8)" << std::endl;;
    }
    if (!(data[i]==0xAA&&data[i+1]==0xAA&&data[i+2]==0xAA&&data[i+3]==0xAA)) {
        std::cout << "Ev " << iev << ": expected header, got " << std::hex << int(data[i]) << "-" << std::hex << int(data[i+1]) << "-" << std::hex << int(data[i+2]) << "-" << std::hex << int(data[i+3]) << std::endl;
    }
    const Int_t length = data[i+7]*256*256*256+data[i+6]*256*256+data[i+5]*256+data[i+4];
    i+=8;
    if (!(i+length<=_len_data)) {
        std::cout << "Ev " << iev << ":Missing data (" << i << " " << length << " " << _len_data << ")" << std::endl;;
    }
    Int_t** event = decode_apts_event(length);
    i+=length;
    if (i+12 <= _len_data && data[i]==0xBB && data[i+1]==0xBB && data[i+2]==0xBB && data[i+3]==0xBB) {
        i+=16;
    }
    iev+=1;
    return event;
}

Int_t** TDecoder::decode_apts_event(const Int_t length) {
    uint8_t d[length];
    std::copy(data+i,data+i+length,d);
    Int_t mapping[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (_mux==kTRUE) {
        std::copy(std::begin(APTS_MUX_PIXEL_ADC_MAPPING),std::end(APTS_MUX_PIXEL_ADC_MAPPING),std::begin(mapping));
    } else {
        std::copy(std::begin(APTS_PIXEL_ADC_MAPPING),std::end(APTS_PIXEL_ADC_MAPPING),std::begin(mapping));
    }
    Int_t nframes = length/40;
    Int_t** event = new Int_t*[nframes];
    for (Int_t frame = 0; frame < nframes; frame++) {
        event[frame]=new Int_t[16];
        Int_t fframe = frame*40;
        Int_t d2temp[16][16];
        for(Int_t j = 0; j < 16; j++) {
            Int_t temp[8] = {0,0,0,0,0,0,0,0};
            Int_t* temp1 = to_binary(temp, d[fframe+2*j]);
            std::copy(temp1,temp1+8,std::begin(d2temp[j]));
            Int_t* temp2 = to_binary(temp, d[fframe+2*j+1]);
            std::copy(temp2,temp2+8,std::begin(d2temp[j])+8);
        }
        for (Int_t j = 0; j < 16; j++) {
            event[frame][mapping[j]]=to_decimal(d2temp[0][j],d2temp[1][j],d2temp[2][j],d2temp[3][j],d2temp[4][j],d2temp[5][j],d2temp[6][j],d2temp[7][j],d2temp[8][j],d2temp[9][j],d2temp[10][j],d2temp[11][j],d2temp[12][j],d2temp[13][j],d2temp[14][j],d2temp[15][j]);
        }
        Int_t trailer = d[fframe+39]+d[fframe+38]*256;
        if (frame < nframes-1) {
            if (trailer!=0xFEFE) std::cout << "Unexpected frame trailer 0x" << std::hex << trailer << " for frame " << frame+1 << " out of " << std::dec << nframes << std::endl;
        } else {
            if (trailer!=0xAEAE) std::cout << "Unexpected event trailer 0x" << std::hex << trailer << " for frame " << frame+1 << " out of " << std::dec << nframes << std::endl;
        }
    }
    return event;
}

Int_t* TDecoder::to_binary(Int_t arr[],Int_t num1) {
    Int_t share1 = num1;
    Int_t rest1 = 0;
    for(Int_t k = 0; k < 8; k++) {
        rest1 = share1%2;
        arr[k] = rest1;
        share1 = ceil(share1/2);
    }
    return arr;
}

Int_t TDecoder::to_decimal(Int_t num1,Int_t num2,Int_t num3,Int_t num4,Int_t num5,Int_t num6,Int_t num7,Int_t num8,Int_t num9,Int_t num10,Int_t num11,Int_t num12,Int_t num13,Int_t num14,Int_t num15,Int_t num16) {
    return num1*32768+num2*16384+num3*8192+num4*4096+num5*2048+num6*1024+num7*512+num8*256+num9*128+num10*64+num11*32+num12*16+num13*8+num14*4+num15*2+num16;
}