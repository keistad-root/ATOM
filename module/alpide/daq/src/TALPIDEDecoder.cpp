#define __TALPIDEDECODER_HEADER__
#include "TALPIDEDecoder.h"

const std::array<uint8_t, 16> stray_sequence{0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

TALPIDEDecoder::TALPIDEDecoder(const std::filesystem::path& binaryPath) : TDecoder(binaryPath) { }
TALPIDEDecoder::TALPIDEDecoder(const std::string& binaryPath) : TDecoder(binaryPath) { }

void TALPIDEDecoder::decode() {
	preTest();
	while ( !isDone() ) {
		inputEvent();
	}
}

std::vector<std::unique_ptr<TALPIDEEvent>> TALPIDEDecoder::getData() {
	return std::move(alpides);
}

void TALPIDEDecoder::inputEvent() {
	int numEvent = hex_to_dec(&*getBinaryData().begin() + index_ + 4, 4);
	long int time = hex_to_dec(&*getBinaryData().begin() + index_ + 8, 8);
	std::unique_ptr<TALPIDEEvent> alpide(new TALPIDEEvent());
	alpide->setEvent(numEvent);
	alpide->setTime(time);
	index_ += 4 * 4;

	if ( bitwise_and(getBinaryData()[index_], 0xF0) == 0xE0 ) { // chip has empty frame
		if ( (getBinaryData()[index_ + 2] != 0xFF) || getBinaryData()[index_ + 3] != 0xFF ) {
			std::cerr << "Assertion Error at empty frame test" << std::endl;
			throw std::bad_exception();
		}
		index_ += 4;
	} else if ( bitwise_and(getBinaryData()[index_], 0xF0) == 0xA0 ) {
		index_ += 2;
		int n = getBinaryData().size();
		int reg = 0;
		while ( index_ < n ) {
			uint8_t data0 = getBinaryData()[index_];
			if ( bitwise_and(data0, 0xC0) == 0x00 ) {  // data  long
				int d = bitwise_or(bitwise_or((reg * pow(2, 14)), bitwise_and(data0, 0x3F) * pow(2, 8)), getBinaryData()[index_ + 1]);
				int x = bitwise_or(bitwise_and((int) (d / pow(2, 9)), 0x3FE), bitwise_and(bitwise_xor(d, (int) (d / pow(2, 1))), 0x1));
				int y = bitwise_and((int) d / pow(2, 1), 0x1FF);
				alpide->pushData({x, y});
				uint8_t data2 = getBinaryData()[index_ + 2];
				d += 1;
				while ( data2 ) {
					if ( data2 & 1 ) {
						int x = bitwise_or(bitwise_and((int) (d / pow(2, 9)), 0x3FE), bitwise_and(bitwise_xor(d, (int) (d / pow(2, 1))), 0x1));
						int y = bitwise_and((int) d / pow(2, 1), 0x1FF);
						alpide->pushData({x, y});
					}
					data2 = (int) (data2 / pow(2, 1));
					d += 1;
				}
				index_ += 3;
			} else if ( bitwise_and(data0, 0xC0) == 0x40 ) {
				int d = bitwise_or(bitwise_or((reg * pow(2, 14)), bitwise_and(data0, 0x3F) * pow(2, 8)), getBinaryData()[index_ + 1]);
				int x = bitwise_or(bitwise_and((int) (d / pow(2, 9)), 0x3FE), bitwise_and(bitwise_xor(d, (int) (d / pow(2, 1))), 0x1));
				int y = bitwise_and((int) d / pow(2, 1), 0x1FF);
				alpide->pushData({x, y});
				index_ += 2;
			} else if ( bitwise_and(data0, 0xE0) == 0xC0 ) {
				reg = bitwise_and(data0, 0x1F);
				index_ += 1;
			} else if ( bitwise_and(data0, 0xF0) == 0xB0 ) {
				index_ += 1;
				index_ = static_cast<int> ((index_ + 3) / 4 * 4);
				break;
			} else if ( data0 == 0xFF ) {
				index_ += 1;
			} else {
				std::cout << "DEBUG: data[i-10,i+10]:";
				for ( int j = 0; j < 20; j++ ) {
					std::cout << getBinaryData()[index_ + j - 10];
				}
				std::cout << "ValueError: invalid literal" << std::endl;
				throw std::bad_exception();
			}
		}
	}
	index_ += 4;
	alpides.push_back(std::move(alpide));
}



void TALPIDEDecoder::preTest() {
	if ( index_ == 0 && strayTest() ) {
		index_ += 16;
	}
	headerTest();
}

bool TALPIDEDecoder::strayTest() {
	if ( std::equal(getBinaryData().begin(), getBinaryData().begin() + 16, stray_sequence.begin()) ) {
		std::cout << "Decoder: stray sequence 0x AA AA AA AA 00 00 00 00 02 00 00 00 00 00 00 00 found. Skipping 16 bytes.";
		return true;
	} else {
		return false;
	}
}

void TALPIDEDecoder::headerTest() {
	if ( !std::equal(getBinaryData().begin(), getBinaryData().begin() + 4, stray_sequence.begin()) ) {
		std::cerr << "Assertion Error in header, not 0x AA AA AA AA" << std::endl;
		throw std::bad_exception();
	}
}

long int TALPIDEDecoder::hex_to_dec(const uint8_t* data, const int& digits) {
	long int val = 0;
	for ( int digit = 0; digit < digits; digit++ ) {
		val += data[digit] * pow(256, digit);
	}
	return val;
}

int TALPIDEDecoder::bitwise_and(int v1, int v2) {
	int result = 0;
	uint8_t digit = 0;
	while ( true ) {
		result += (v1 % 2 * v2 % 2) * pow(2, digit);
		v1 = v1 / 2;
		v2 = v2 / 2;
		digit++;
		if ( v1 == 0 || v2 == 0 ) break;
	}
	return result;
}

int TALPIDEDecoder::bitwise_or(int v1, int v2) {
	int result = 0;
	uint8_t digit = 0;
	while ( true ) {
		result -= ((v1 % 2 - 1) * (v2 % 2 - 1) - 1) * pow(2, digit);
		v1 = v1 / 2;
		v2 = v2 / 2;
		digit++;
		if ( v1 == 0 && v2 == 0 ) break;
	}
	return result;
}

int TALPIDEDecoder::bitwise_xor(int v1, int v2) {
	int result = 0;
	uint8_t digit = 0;
	while ( true ) {
		result += (v1 % 2 + v2 % 2) % 2 * pow(2, digit);
		v1 = v1 / 2;
		v2 = v2 / 2;
		digit++;
		if ( v1 == 0 && v2 == 0 ) break;
	}
	return result;
}

bool TALPIDEDecoder::isDone() {
	if ( index_ >= getDataLength() ) return true;
	else return false;
}