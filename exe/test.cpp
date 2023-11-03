#include <iostream>
#include <filesystem>
#include <TROOT.h>

int main(int argc, char** argv) {
    std::filesystem::path path(argv[1]);
    TString str = path.string();

    std::cout << str << std::endl;
}