#include "TALPIDE.h"
#include "TALPIDEDecoder.h"

int main() {
    std::filesystem::path inputPath("data/fhrscan-20231229_180426.raw");
    TALPIDEDecoder* decoder = new TALPIDEDecoder(inputPath);
    decoder->readFile();
    decoder->decode();

    std::vector<TALPIDE*> alpides = decoder->getData();
    for ( TALPIDE* alpide : alpides ) {
        std::cout << alpide->getEvent() << " " << alpide->getTime() << " ";
        for ( std::array<int,2> data : alpide->getData()) {
            std::cout << data[0] << " " << data[1] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}