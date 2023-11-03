#include "pitch_clock.h"

TClock::TClock() {
    start = clock();
}

void TClock::Measure() {
    finish = clock();
    duration = (double)(finish-start) / CLOCKS_PER_SEC;
    std::cout << "Time from the start flows " << duration << "s" << std::endl;
}

void TClock::EndProgram() {
    finish = clock();
    duration = (double)(finish-start) / CLOCKS_PER_SEC;
    std::cout << "Total run time of this program is " << duration << "s" << std::endl;
    std::cout << "Good bye!" << std::endl;
}