#include "cppconfig.h"

Configurable::Configurable(std::string_view configName) : mConfigName(configName) { }

Configurable::~Configurable() { }

const std::string& Configurable::getConfigName() const {
	return mConfigName;
}

std::unordered_map<std::string, std::string> Configurable::getDictionary() {
	return mDictionary;
}


void Configurable::addDictionary(std::string_view key, std::string_view value) {
	mDictionary[std::string(key)] = std::string(value);
}

const bool Configurable::hasKey(std::string_view key) const {
	if ( mDictionary.count(std::string(key)) ) {
		return true;
	} else {
		return false;
	}
}

const std::string& Configurable::find(std::string_view key) const {
	try {
		if ( !mDictionary.count(std::string(key)) ) {
			throw ConfigrableNoValue(key, mConfigName);
		} else {
			return mDictionary.at(std::string(key));
		}
	} catch ( ConfigrableNoValue& e ) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

const std::vector<std::string> Configurable::findlist(std::string_view key) const {
	try {
		if ( !mDictionary.count(std::string(key)) ) {
			throw ConfigrableNoValue(key, mConfigName);
		} else {
			std::vector<std::string> values;
			std::string value;
			std::istringstream valueStream(mDictionary.at(std::string(key)));
			while ( std::getline(valueStream, value, '\n') ) {
				values.push_back(value);
			}
			return values;
		}
	} catch ( ConfigrableNoValue& e ) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}

Configuration::Configuration(std::string_view configFile) {
	addConfig(configFile);
}

void Configuration::addConfig(std::string_view configFile) {
	if ( !std::filesystem::exists(configFile) ) {
		std::cerr << "File " << configFile << " cannot be found." << std::endl;
		throw std::runtime_error("Configuration not found");
	}
	std::ifstream conf(configFile);
	std::string line;
	std::string key;
	std::string value;
	while ( getline(conf, line) ) {
		line.erase(remove(line.begin(), line.end(), '\t'), line.end());
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		if ( line[0] == '#' || line == "" ) {
			continue;
		} else if ( line[0] == '[' ) {
			mConfigs.push_back(Configurable(line.substr(1, line.length() - 2)));
		} else {
			key = line.substr(0, line.find("="));
			// key.erase(remove(key.begin(), key.end(), '\t'), key.end());
			// key.erase(remove(key.begin(), key.end(), ' '), key.end());

			if ( line.find('{') == std::string::npos ) {
				value = line.substr(line.find("=") + 1);
				// value.erase(remove(value.begin(), value.end(), '\t'), value.end());
				// value.erase(remove(value.begin(), value.end(), ' '), value.end());

				mConfigs.back().addDictionary(key, value);
				key = "";
				value = "";
			} else {
				while ( getline(conf, line) ) {
					line.erase(remove(line.begin(), line.end(), '\t'), line.end());
					line.erase(remove(line.begin(), line.end(), ' '), line.end());
					if ( line[0] == '#' || line == "" ) {
						continue;
					} else if ( line.find('}') != std::string::npos ) {
						mConfigs.back().addDictionary(key, value);
						key = "";
						value = "";
						break;
					} else {
						line.erase(remove(line.begin(), line.end(), ','), line.end());
						value += line + "\n";
					}

				}
			}
		}
	}
	conf.close();
}

const std::vector<std::string> Configuration::getConfigurableNameList() const {
	std::vector<std::string> configurableNameList;
	for ( const Configurable& config : mConfigs ) {
		configurableNameList.push_back(config.getConfigName());
	}
	return configurableNameList;
}

std::optional<Configurable> Configuration::getConfig(std::string_view configTitle) const {
	for ( auto& config : mConfigs ) {
		if ( config.getConfigName() == configTitle ) {
			return config;
		}
	}
	return std::nullopt;
}

std::ostream& operator<<(std::ostream& os, const Configurable& copy) {
	if ( copy.getConfigName().length() < 8 ) {
		os << copy.getConfigName() << "\t\t";
	} else if ( copy.getConfigName().length() < 16 ) {
		os << copy.getConfigName() << "\t";
	} else {
		os << copy.getConfigName() << std::endl << "\t\t";
	}
	for ( auto& dict : copy.mDictionary ) {
		os << dict.first << ", ";
	}
	os << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Configuration& copy) {
	for ( auto& config : copy.mConfigs ) {
		os << config;
	}
	return os;
}

Configuration::~Configuration() { }