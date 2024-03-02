#include "cppconfig.h"

Configurable::Configurable(const std::string& configName) : configName_(configName) {}

Configurable::~Configurable() {}

const std::string& Configurable::getConfigName() const {
    return configName_;
}

void Configurable::addDictionary(const std::string& key, const std::string& value) {
    dictionary_.insert_or_assign(key,value);
}

const std::string& Configurable::find(const std::string& key) const {
    static const std::string emptyString;
    if (dictionary_.count(key)) {
        return dictionary_.at(key);
    } else {
        return emptyString;
    }
}

Configuration::Configuration(const std::string& configFile) {
    addConfig(configFile);
}

void Configuration::addConfig(const std::string& configFile) {
    if (!std::filesystem::exists(configFile)) {
        std::cerr << "File " << configFile << " cannot be found." << std::endl;
        throw std::runtime_error("Configuration not found");
    }
    std::ifstream conf(configFile);
    std::string line;
    while (getline(conf,line)) {
        if (line[0] == '#' || line == "") {
            continue;
        } else if (line[0] == '[') {
            configs_.push_back(Configurable(line.substr(1,line.length()-2)));
        } else {
            std::string key = line.substr(0,line.find("="));
            std::string value = line.substr(line.find("=")+1);

            key.erase(remove(key.begin(),key.end(),'\t'),key.end());
            key.erase(remove(key.begin(),key.end(),' '),key.end());

            value.erase(remove(value.begin(),value.end(),'\t'),value.end());
            value.erase(remove(value.begin(),value.end(),' '),value.end());
            configs_.back().addDictionary(key,value);
        }
    }
    conf.close();
}

const Configurable& Configuration::getConfig(const std::string& configTitle) const {
    for (auto& config : configs_) {
        if (config.getConfigName() == configTitle) {
            return config;
        }
    }
    throw std::runtime_error("Configuration not found");
}

std::ostream& operator<<(std::ostream& os, const Configurable& copy) {
    if (copy.getConfigName().length() < 8) {
        os << copy.getConfigName() << "\t\t";
    } else if (copy.getConfigName().length() < 16) {
        os << copy.getConfigName() << "\t";
    } else {
        os << copy.getConfigName() << std::endl <<"\t\t";
    }
    for (auto& dict : copy.dictionary_) {
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