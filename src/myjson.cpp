#include "myjson.h"

inputJSON::inputJSON() {}

inputJSON::inputJSON(std::string fileName) {
    file.open(fileName);
    std::string line;
    while(!file.eof())
    {
        getline(file,line);
        std::string name, value;
        if (line.find("\"") != 18446744073709551615){
            std::string line2 = line.substr(line.find("\"")+1);
            name = line2.substr(0,line2.find("\""));
            std::string line3 = line2.substr(line2.find(":")+2);
            if (line3[0]=='[') {
                getline(file,line);
                value = line.substr(line.find_first_not_of(" "));
            }
            else if (line3[0]=='\"') {
                value = line3.substr(line3.find("\"")+1,line3.rfind("\"")-1);
            }
            else {
                value = line3.substr(0, line3.find(","));
            }
            objects.emplace_back(name, value);
        }
    }
}

inputJSON::~inputJSON() {
    file.close();
    std::cout << "JSON file Closed" << std::endl;
}

void inputJSON::jsonOpen(std::string fileName) {
    file.open(fileName);
    std::string line;
    while(!file.eof())
    {
        getline(file,line);
        std::string name, value;
        if (line.find("\"") != 18446744073709551615){
            std::string line2 = line.substr(line.find("\"")+1);
            name = line2.substr(0,line2.find("\""));
            std::string line3 = line2.substr(line2.find(":")+2);
            if (line3[0]=='[') {
                getline(file,line);
                value = line.substr(line.find_first_not_of(" "));
            }
            else if (line3[0]=='\"') {
                value = line3.substr(line3.find("\"")+1,line3.rfind("\"")-1);
            }
            else {
                value = line3.substr(0, line3.find(","));
            }
            objects.emplace_back(name, value);
        }
    }

}

TString inputJSON::findValue(TString name) {
    for (auto object:objects){
        if (object.getName()==name) {
            return object.getValue();
        }
    }
    return "No name in json file";
}