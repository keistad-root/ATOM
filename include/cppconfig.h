#ifndef __CPPCONFIG__
#define __CPPCONFIG__

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

class Configurable {
private:
    std::string configName_;
    std::unordered_map<std::string,std::string> dictionary_;
public:
    Configurable(const std::string& configName);
    ~Configurable();
    void addDictionary(const std::string& key, const std::string& value);
    const std::string& getConfigName() const;
    const std::string& find(const std::string& key) const;

    friend std::ostream& operator<<(std::ostream& os, const Configurable& copy);

};

class Configuration {
private:
    std::vector<Configurable> configs_;
public:
    Configuration(const std::string& configFile);
    ~Configuration();
    void addConfig(const std::string& configFile);
    const Configurable& getConfig(const std::string& configTitle) const;
    friend std::ostream& operator<<(std::ostream& os, const Configuration& copy);
};


#endif