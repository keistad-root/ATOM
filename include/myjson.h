#ifndef __MYJSON_H__
#define __MYJSON_H__

#include <iostream>
#include <fstream>
#include <vector>
#include "TROOT.h"

class Object {
private:
    TString name;
    TString value;
public:
    Object(TString _name, TString _value) {
        name = _name;
        value = _value;
    }
    void const print() { std::cout << name << " " << value << std::endl; }
    TString getName() { return name; }
    TString getValue() { return value; }
};

class inputJSON {
private:
    std::vector<Object> objects;
    std::ifstream file;
public:
    inputJSON();
    inputJSON(std::string fileName);
    ~inputJSON();
    void jsonOpen(std::string fileName);
    TString findValue(TString name);
};


#endif