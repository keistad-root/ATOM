#include "cppconfig.h"
#include <typeinfo>

Configurable::Configurable(std::string_view configName) : mConfigName(configName) {}

Configurable::~Configurable() {}

const std::string& Configurable::getConfigName() const {
    return mConfigName;
}

void Configurable::addDictionary(std::string_view key, std::string_view value) {
    mDictionary[std::string(key)] = std::string(value);

}

const std::string& Configurable::find(std::string_view key) const {
    static const std::string emptyString;
    if (mDictionary.count(std::string(key))) {
        return mDictionary.at(std::string(key));
    } else {
        return emptyString;
    }
}

Configuration::Configuration(std::string_view configFile) {
    addConfig(configFile);
}

void Configuration::addConfig(std::string_view configFile) {
    if (!std::filesystem::exists(configFile)) {
        std::cerr << "File " << configFile << " cannot be found." << std::endl;
        throw std::runtime_error("Configuration not found");
    }
    std::ifstream conf(configFile);
    std::string line;
    std::string key;
    std::string value;
    while (getline(conf,line)) {
        if (line[0] == '#' || line == "") {
            continue;
        } else if (line[0] == '[') {
            configs_.push_back(Configurable(line.substr(1,line.length()-2)));
        } else {
            key = line.substr(0,line.find("="));
            key.erase(remove(key.begin(),key.end(),'\t'),key.end());
            key.erase(remove(key.begin(),key.end(),' '),key.end());

            if (line.find('{') == std::string::npos) {
                value = line.substr(line.find("=")+1);
                value.erase(remove(value.begin(),value.end(),'\t'),value.end());
                value.erase(remove(value.begin(),value.end(),' '),value.end());
                
                configs_.back().addDictionary(key,value);
                key = "";
                value = "";
            } else {
                while (getline(conf, line)) {
                    if (line.find('}') != std::string::npos) { 
                        configs_.back().addDictionary(key, value);
                        key = "";
                        value = "";
                        break; 
                    }
                    value += line + "\n";
                }
            }   
        }
    }
    conf.close();
}

std::optional<Configurable> Configuration::getConfig(std::string_view configTitle) const {
    for (auto& config : configs_) {
        if (config.getConfigName() == configTitle) {
            return config;
        }
    }
    return std::nullopt;
}

std::ostream& operator<<(std::ostream& os, const Configurable& copy) {
    if (copy.getConfigName().length() < 8) {
        os << copy.getConfigName() << "\t\t";
    } else if (copy.getConfigName().length() < 16) {
        os << copy.getConfigName() << "\t";
    } else {
        os << copy.getConfigName() << std::endl <<"\t\t";
    }
    for (auto& dict : copy.mDictionary) {
        os << dict.first << ", ";
    }
    os << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Configuration& copy) {
    for (auto& config : copy.configs_) {
        os << config;
    }
    return os;
}

Configuration::~Configuration() {}