#include "cppTimer.h"

TTimer::TTimer() {
    start = clock();
}

void TTimer::Measure() {
    finish = clock();
    duration = (double)(finish-start) / CLOCKS_PER_SEC;
    std::cout << "Time from the start flows " << duration << "s" << std::endl;
}

void TTimer::EndProgram() {
    finish = clock();
    duration = (double)(finish-start) / CLOCKS_PER_SEC;
    std::cout << "Total run time of this program is " << duration << "s" << std::endl;
    std::cout << "Good bye!" << std::endl;
}