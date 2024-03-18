#include "TAPTSDecoder.h"

TAPTSDecoder::TAPTSDecoder(const std::filesystem::path& binaryPath) : TDecoder(binaryPath) {}
TAPTSDecoder::TAPTSDecoder(const std::string& binaryPath) : TDecoder(binaryPath) {}

void TAPTSDecoder::decode() {
    mux_ ? std::copy(std::begin(APTS_MUX_PIXEL_ADC_MAPPING), std::end(APTS_MUX_PIXEL_ADC_MAPPING), mapping.begin()) 
         : std::copy(std::begin(APTS_PIXEL_ADC_MAPPING), std::end(APTS_PIXEL_ADC_MAPPING), mapping.begin());
    // Set mapping array. 

    while (!isDone()) {
        inputEvent();
    }
}

std::vector<TAPTSEvent*> TAPTSDecoder::getData() {
    return aptss;
}

void TAPTSDecoder::inputEvent() {
    preTest();

    std::vector<uint8_t> eventData(getBinaryData().begin()+index_,getBinaryData().begin()+index_+lenEvent_);
    // A part of binary data.

    nFrame_ = lenEvent_ / 40;
    // Set number of frame.

    for (int iFrame = 0; iFrame < nFrame_; iFrame++) {
        TAPTSEvent* apts = new TAPTSEvent();
        apts->setEvent(iEvent_);
        for (int j = 0; j < 16; j++) {
            int share1 = eventData[iFrame * 40 + 2 * j];
            int share2 = eventData[iFrame * 40 + 2 * j + 1];
            int rest1 = 0;
            int rest2 = 0;
            for (int digit = 0; digit < 8; digit++) {
                rest1 = share1 % 2;
                rest2 = share2 % 2;
                apts->getData()[mapping[digit]] += rest1 * pow(2, 15 - j);
                apts->getData()[mapping[digit + 8]] += rest2 * pow(2, 15 - j);
                share1 = ceil(share1 / 2);
                share2 = ceil(share2 / 2);
            }
        }
        apts->setFrame(iFrame);

        int trailer = eventData[iFrame * 40 + 39] + eventData[iFrame * 40 + 38] * 256;
        if (iFrame < nFrame_ - 1) {
            if (trailer!=0xFEFE) std::cout << "Unexpected frame trailer 0x" << std::hex << trailer << " for frame " << iFrame+1 << " out of " << std::dec << nFrame_ << std::endl;
        } else {
            if (trailer!=0xAEAE) std::cout << "Unexpected event trailer 0x" << std::hex << trailer << " for frame " << iFrame+1 << " out of " << std::dec << nFrame_ << std::endl;
        }
        aptss.push_back(apts);
    }
    index_ += lenEvent_;
    postTest();
    iEvent_++;
}

void TAPTSDecoder::postTest() {
    if (index_ + 12 <= getDataLength() && std::equal(getBinaryData().begin() + index_, getBinaryData().begin() + index_ + 3,expected_footer.begin())) {
        index_ += 16;
    }
}

void TAPTSDecoder::preTest() {
    rangeTest();
    headerTest();
    lenEvent_ = getEventLength();
    index_ += 8;
    missingTest();
}

void TAPTSDecoder::rangeTest() {
    if (!(index_+8 <= getDataLength())) {
        std::cerr << "Ev " << iEvent_ << ": Unexpected data length(" << getDataLength() - 1 << " < 8)" << std::endl;
    }
}

void TAPTSDecoder::headerTest() {
    if (!std::equal(getBinaryData().begin()+index_,getBinaryData().begin()+index_+3,expected_header.begin())) {
        std::cout << "Ev " << iEvent_ << std::hex << int(getBinaryData()[index_]) << "-" << int(getBinaryData()[index_+1]) << "-" << int(getBinaryData()[index_+2]) << "-" << int(getBinaryData()[index_+3]) << std::endl;
    }
}

void TAPTSDecoder::missingTest() {
    if (!(index_ + lenEvent_ <= getDataLength())) {
        std::cerr << "Ev " << iEvent_ << ":Missing data (" << index_ << " " << lenEvent_ << " " << getDataLength() << ")" << std::endl;
    }
}

int TAPTSDecoder::getEventLength() {
    int lenEvent_ = 0;
    for (int i = 0; i < 4; i++) {
        lenEvent_ += getBinaryData()[index_ + 4 + i] * pow(256, i);
    }
    return lenEvent_;
}

bool TAPTSDecoder::isDone() {
    if (index_ >= getDataLength()) return true;
    else return false;
}