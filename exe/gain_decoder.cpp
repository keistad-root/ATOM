#include <iostream>
#include <filesystem>

#include "TROOT.h"

#include "myjson.h"
#include "gain_decoder.h"
#include "pitch_clock.h"

TString SetInputName(char* executable, char* str) {
    std::filesystem::path path;
    std::filesystem::path current(executable);
    if (str == NULL) {
        path = std::filesystem::canonical(current.parent_path()).string() + "/data/";
    } else {
        path = str;
    }
    if (std::filesystem::is_regular_file(path)) {
        if (path.extension() == ".raw") {
            return path.string();
        } else {
            return path.replace_filename(path.stem()).string() + ".raw";
        }
    } else if (path.parent_path().filename().string() == "gain") {
        std::filesystem::directory_iterator itr(path / "");
        while (itr != std::filesystem::end(itr)) {
            const std::filesystem::directory_entry& entry = *itr;
            if (entry.path().filename().extension().string() == ".raw") {
                return entry.path().string();
                break;
            }
            itr++;
        }
    } else {
        std::filesystem::directory_iterator itr(path / "gain");
        while (itr != std::filesystem::end(itr)) {
            const std::filesystem::directory_entry& entry = *itr;
            if (entry.path().filename().extension().string() == ".raw") {
                return entry.path().string();
                break;
            }
            itr++;
        }
    } 
    return nullptr;
}

TString SetOutputName(char* executable, char* str) {
    std::filesystem::path path;
    std::filesystem::path current(executable);
    if (str == NULL) {
        path = std::filesystem::canonical(current.parent_path()).string() + "/data/";
    } else {
        path = str;
    }

    if (std::filesystem::is_regular_file(path)) {
        return path.filename().stem().string();
    } else {
        return path.string() + "apts_gain";
    }
}

std::string SetJsonName(char* executable, char* str) {
    std::filesystem::path path;
    std::filesystem::path current(executable);
    if (str == NULL) {
        path = std::filesystem::canonical(current.parent_path()).string() + "/data/";
    } else {
        path = str;
    }
    if (std::filesystem::is_regular_file(path)) {
        if (path.extension() == ".json") {
            return path.string();
        } else {
            return path.replace_filename(path.stem()).string() + ".json";
        }
    } else if (path.parent_path().filename().string() == "gain") {
        std::filesystem::directory_iterator itr(path / "");
        while (itr != std::filesystem::end(itr)) {
            const std::filesystem::directory_entry& entry = *itr;
            if (entry.path().filename().extension().string() == ".json") {
                return entry.path().string();
                break;
            }
            itr++;
        }
    } else {
        std::filesystem::directory_iterator itr(path / "gain");
        while (itr != std::filesystem::end(itr)) {
            const std::filesystem::directory_entry& entry = *itr;
            if (entry.path().filename().extension().string() == ".json") {
                return entry.path().string();
                break;
            }
            itr++;
        }
    } 
    return nullptr;
}

int main(int argc, char** argv) {
    TClock* clock = new TClock();

    std::cout << "Gain decoding..." << std::endl;
    
    inputJSON* json = new inputJSON(SetJsonName(argv[0],argv[2]));
    GainDecoder* dec = new GainDecoder(SetInputName(argv[0],argv[1]), SetOutputName(argv[0],argv[3]));

    std::cout << "Decoded file generated..." << std::endl;
    dec->MakeDecodedFile();
    clock->Measure();
    
    std::cout << "Analysed file generated..." << std::endl;
    dec->CalAnalysedData(json->findValue("ntrg_vres").Atoi());
    dec->MakeAnalysedData();
    clock->Measure();

    std::cout << "Control plots are being drawn..." << std::endl;
    dec->DrawControlPlot();
    clock->EndProgram();

    return 0;
}