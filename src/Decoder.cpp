#include "decoder.h"

TDecoder::TDecoder() {
}

TDecoder::TDecoder(TString file_in, Bool_t mux) : file_in_{file_in}, data_index_{0}, event_num_{0}, mux_{mux} {
    OpenBinary(file_in_); // Open Bianry File
    ReadBinary(); // Read Binary File and save to binary_data_ variable.
}

TDecoder::TDecoder(TString file_in, TString file_out, Bool_t mux) : file_in_{file_in}, data_index_{0}, event_num_{0}, mux_{mux} {
    outputFile_ = new TFile(file_out,"RECREATE");
    OpenBinary(file_in_); // Open Bianry File
    ReadBinary(); // Read Binary File and save to binary_data_ variable.
}

TDecoder::TDecoder(TString file_in, TFile* outputFile, Bool_t mux) : file_in_{file_in}, data_index_{0}, event_num_{0}, mux_{mux} {
    outputFile_ = outputFile;
    OpenBinary(file_in_); // Open Bianry File
    ReadBinary(); // Read Binary File and save to binary_data_ variable.
}

TDecoder::~TDecoder() {
    delete binary_data_;
}

void TDecoder::OpenBinary() {
    rawFile_.open(file_in_, std::ios::binary); // Open binary file with class member.
}

void TDecoder::OpenBinary(TString file_in) {
    rawFile_.open(file_in, std::ios::binary); // Open binary file with function parameter.
}

void TDecoder::ReadBinary() {
    char buf[sizeof(uint8_t)];
    std::vector<uint8_t> values;
    while (rawFile_.read(buf, sizeof(buf))) {
        values.push_back(0); 
        memcpy(&values[event_num_], buf, sizeof(values[event_num_]));
        event_num_++;
    }
    binary_data_ = new uint8_t[values.size()];
    std::copy(values.begin(), values.end(), binary_data_);
    len_data_ = values.size();

    values.clear();
    std::vector<uint8_t>().swap(values);
}

Bool_t TDecoder::IsDone() {
    if (data_index_ >= len_data_) return kTRUE;
    else return kFALSE;
}

Int_t** TDecoder::GetNextEvent() {
    if (!(data_index_+8 <= len_data_)) {
        std::cout << "Ev " 
                  << event_num_ 
                  << ": Unexpected data length (" 
                  << len_data_-1 
                  << " < 8)" 
                  << std::endl;;
    } // Test data length

    if (!(binary_data_[data_index_] == 0xAA &&
          binary_data_[data_index_ + 1] == 0xAA &&
          binary_data_[data_index_ + 2] == 0xAA &&
          binary_data_[data_index_ + 3] == 0xAA)) {
        std::cout << "Ev " 
                  << event_num_ 
                  << ": expected header, got " 
                  << std::hex 
                  << int(binary_data_[data_index_]) 
                  << "-" 
                  << int(binary_data_[data_index_ + 1]) 
                  << "-" 
                  << int(binary_data_[data_index_ + 2]) 
                  << "-" 
                  << int(binary_data_[data_index_ + 3]) 
                  << std::endl;
    } // Test header of data

    len_event_data_ = binary_data_[data_index_ + 7] * pow(256,3) + binary_data_[data_index_ + 6] * pow(256,2) + binary_data_[data_index_ + 5] * 256 + binary_data_[data_index_ + 4];
    data_index_+=8;

    if (!(data_index_ + len_event_data_ <= len_data_)) {
        std::cout << "Ev " 
                  << event_num_ 
                  << ":Missing data (" 
                  << data_index_ 
                  << " " 
                  << len_event_data_
                  << " " 
                  << len_data_ 
                  << ")" 
                  << std::endl;;
    } // Test existing of missing data

    DecodeAptsEvent();
    data_index_ += len_event_data_;
    
    if (data_index_ + 12 <= len_data_ && 
        binary_data_[data_index_] == 0xBB && 
        binary_data_[data_index_ + 1] == 0xBB && 
        binary_data_[data_index_ + 2] == 0xBB && 
        binary_data_[data_index_ + 3] == 0xBB) {
        data_index_ += 16;
    }

    event_num_+=1;
    return event_data_;
}

void TDecoder::DecodeAptsEvent() {
    uint8_t temp_data[len_event_data_];
    std::copy(binary_data_ + data_index_, binary_data_ + data_index_ + len_event_data_, temp_data);
    // Copy temp_data for dealing the data more easily.

    Int_t mapping[16] = {0,};
    if (mux_ == kTRUE) {
        std::copy(std::begin(APTS_MUX_PIXEL_ADC_MAPPING), std::end(APTS_MUX_PIXEL_ADC_MAPPING), std::begin(mapping));
    } else {
        std::copy(std::begin(APTS_PIXEL_ADC_MAPPING), std::end(APTS_PIXEL_ADC_MAPPING), std::begin(mapping));
    } 

    Int_t nframes = len_event_data_ / 40;

    event_data_ = new Int_t*[nframes];
    
    for (Int_t frame = 0; frame < nframes; frame++) {
        event_data_[frame] = new Int_t[16];
        Int_t fframe = frame*40;
        Int_t temp[16][16];

        for(Int_t j = 0; j < 16; j++) {
            Int_t* temp1 = to_binary(temp_data[fframe + 2 * j]);
            std::copy(temp1,temp1+8,std::begin(temp[j]));
            Int_t* temp2 = to_binary(temp_data[fframe + 2 * j + 1]);
            std::copy(temp2,temp2+8,std::begin(temp[j])+8);
        }

        for (Int_t j = 0; j < 16; j++) {
            event_data_[frame][mapping[j]] = to_decimal(temp[0][j], temp[1][j], temp[2][j], temp[3][j], temp[4][j], temp[5][j], temp[6][j], temp[7][j], temp[8][j], temp[9][j], temp[10][j], temp[11][j], temp[12][j], temp[13][j], temp[14][j], temp[15][j]);
        }

        Int_t trailer = temp_data[fframe + 39] + temp_data[fframe + 38] * 256;
        if (frame < nframes-1) {
            if (trailer!=0xFEFE) std::cout << "Unexpected frame trailer 0x" << std::hex << trailer << " for frame " << frame+1 << " out of " << std::dec << nframes << std::endl;
        } else {
            if (trailer!=0xAEAE) std::cout << "Unexpected event trailer 0x" << std::hex << trailer << " for frame " << frame+1 << " out of " << std::dec << nframes << std::endl;
        }
    }
}

Int_t* TDecoder::to_binary(Int_t num1) {
    Int_t binary_num[8] = {0,};
    Int_t share1 = num1;
    Int_t rest1 = 0;
    for(Int_t digit = 0; digit < 8; digit++) {
        rest1 = share1%2;
        binary_num[digit] = rest1;
        share1 = ceil(share1/2);
    }
    return binary_num;
}

Int_t TDecoder::to_decimal(Int_t num1, Int_t num2, Int_t num3, Int_t num4, Int_t num5, Int_t num6, Int_t num7, Int_t num8, Int_t num9, Int_t num10, Int_t num11, Int_t num12, Int_t num13, Int_t num14, Int_t num15, Int_t num16) {
    return num1 * 32768 + num2 * 16384 + num3 * 8192 + num4 * 4096 + num5 * 2048 + num6 * 1024 + num7 * 512 + num8 * 256 + num9 * 128 + num10 * 64 + num11 * 32 + num12 * 16 + num13 * 8 + num14 * 4 + num15 * 2 + num16;
}

void TDecoder::raw_to_root() {
    tree = new TTree("data", "APTS data");
    Int_t nframe;
    Int_t signal[4][4];

    tree->Branch("Number of Event",&event_num_,"nev/I");
    tree->Branch("Number of Frame",&nframe,"nf/I");
    tree->Branch("signal",signal,"sig[4][4]/I");

    while (!IsDone()) {
        Int_t** data = GetNextEvent();
        for (Int_t frame = 0; frame < 200; frame++){
            nframe=frame;
            for (Int_t y = 0; y < 4; y++) {
                for (Int_t x = 0; x < 4; x++) {
                    signal[x][y]=data[frame][y+x*4];
                }
            }
            tree->Fill();
        }
    }
    tree->Write();
}

void TDecoder::CloseOutputFile() {
    outputFile_->Close();
    delete outputFile_;
}