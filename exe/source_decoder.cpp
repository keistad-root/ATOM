#include <iostream>
#include <string>
#include <ctime>
<<<<<<< HEAD
=======
#include <filesystem>
>>>>>>> 1b4da75fa6695e54d814bc665eea7cd5357e777a

#include "TFile.h"

#include "decoder.h"
#include "myjson.h"
#include "pitch_clock.h"

<<<<<<< HEAD
TString* extract_name_volt(TString chip_ID) {
    TString str[2] = {"a","b"};
    return str;
}

void decoder(TString file_in, TString file_out="data/apts") {
    TFile* outputFile = new TFile(file_out+".root","RECREATE");
    inputJSON* jsonFile = new inputJSON(std::string(file_in.Data())+".json");
    TString chip_ID = jsonFile->findValue("chip_ID");
    TString* nameVolt = extract_name_volt(chip_ID);
    Bool_t mux = kFALSE;
    TDecoder* dec = new TDecoder(file_in+".raw",outputFile,mux);

    dec -> raw_to_root();

    delete jsonFile;
=======
void decoder(TString file_in, TString file_out="data/apts") {
    TFile* outputFile = new TFile(file_out,"RECREATE");
    Bool_t mux = kFALSE;
    TDecoder* dec = new TDecoder(file_in,outputFile,mux);

    dec -> raw_to_root();

>>>>>>> 1b4da75fa6695e54d814bc665eea7cd5357e777a
    outputFile->Close();
    delete outputFile;
    delete dec;
}

<<<<<<< HEAD
int main() {
    TClock* clock = new TClock();
    std::cout << "Source Decoding..." << std::endl;

    TString path = "data/";
    decoder(path + "source/apts_20230130_172940");
=======
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
    } else if (path.parent_path().filename().string() == "source") {
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
        std::filesystem::directory_iterator itr(path / "source");
        while (itr != std::filesystem::end(itr)) {
            const std::filesystem::directory_entry& entry = *itr;
            if (entry.path().filename().extension().string() == ".raw") {
                return entry.path().string();
                break;
            }
            itr++;
        }
    } 
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
        return path.string();
    } else {
        return path.string() + "apts.root";
    }
}

int main(int argc, char** argv) {
    TClock* clock = new TClock();
    std::cout << "Source Decoding..." << std::endl;
    std::cout << SetInputName(argv[0], argv[1]) << std::endl;
    decoder(SetInputName(argv[0], argv[1]), SetOutputName(argv[0], argv[2]));
>>>>>>> 1b4da75fa6695e54d814bc665eea7cd5357e777a

    clock->EndProgram();
    return 0;
}