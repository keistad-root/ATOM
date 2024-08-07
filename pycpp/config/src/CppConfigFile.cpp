#include "CppConfigFile.h"

/**
 * @brief Construct a new CppConfigFile::CppConfigFile object
 *
*/
CppConfigFile::CppConfigFile() = default;

/**
 * @brief Construct a new CppConfigFile::CppConfigFile object
 * @details Taking config file path and store config dictionaries.
 *
 * @param configFile
*/
CppConfigFile::CppConfigFile(std::string_view configFile) {
	// Find config file. If the file doens't exist, then it omits error message.
	try {
		if ( !std::filesystem::exists(configFile) ) { throw CppConfigFileError("FileExist", configFile); }
		addConfig(configFile);
	} catch ( CppConfigFileError error ) {
		error.what();
		exit(1);
	}
}

CppConfigFile::CppConfigFile(const CppConfigFile& copy) {
	for ( const CppConfigDictionary& dict : copy.mConfigs ) {
		mConfigs.push_back(dict);
	}
}
CppConfigFile& CppConfigFile::operator=(const CppConfigFile& copy) {
	for ( const CppConfigDictionary& dict : copy.mConfigs ) {
		mConfigs.push_back(dict);
	}
	return *this;
}
CppConfigFile::CppConfigFile(CppConfigFile&& copy) {
	for ( const CppConfigDictionary& dict : copy.mConfigs ) {
		mConfigs.push_back(dict);
	}
}
CppConfigFile& CppConfigFile::operator=(CppConfigFile&& copy) {
	for ( const CppConfigDictionary& dict : copy.mConfigs ) {
		mConfigs.push_back(dict);
	}
	return *this;
}

/**
 * @brief Add config dictionaries from config file
 *
 * @param configFile
*/
void CppConfigFile::addConfig(std::string_view configFile) {
	// Open config file
	std::ifstream conf(configFile);

	// The reading line
	std::string line;
	// The key 
	std::string key;
	// The value is stored as std::string form
	std::string value;

	// Read config file line by line
	std::vector<std::string> valueArray;
	bool isFirst = true;
	while ( getline(conf, line) ) {
		// Remove comment out part
		std::string result;
		bool escapeNext = false;

		for ( char c : line ) {
			if ( escapeNext ) {
				if ( c == '#' ) {
					result += '#';
				} else {
					result += '\\';
					result += c;
				}
				escapeNext = false;
			} else {
				if ( c == '\\' ) {
					escapeNext = true;
				} else {
					result += c;
				}
			}
		}
		if ( escapeNext ) {
			result += '\\'; // Add trailing backslash if input ends with a backslash
		}
		line = result;
		line.erase(remove(line.begin(), line.end(), '\t'), line.end());
		if ( line[0] == '#' ) {
			line = "";
		}
		if ( line.find('=') != std::string::npos ) {
			line.erase(remove(line.begin(), line.begin() + line.find('='), ' '), line.begin() + line.find('='));
		} else {
			line.erase(remove(line.begin(), line.end(), ' '), line.end());
		}
		if ( line == "" ) { // Pass the blank line
			continue;
		} else if ( line.find('[') != std::string::npos && (line[line.find('[') - 1] != '\\') ) { // The line started with '[' is considered as Configurable name
			if ( !isFirst ) {
				mConfigs.push_back(getConfigFromArray(key, valueArray));
				valueArray.clear();
			} else {
				isFirst = false;
			}
			key = line.substr(line.find('[') + 1, line.find(']') - 1);
			// Make new configurable  
		} else {
			valueArray.push_back(line);
		}
	}
	if ( valueArray.size() != 0 ) {
		mConfigs.push_back(getConfigFromArray(key, valueArray));
	}
}

void CppConfigFile::addConfig(std::string_view configTitle, const std::vector<std::string>& configArray) {
	mConfigs.push_back(getConfigFromArray(configTitle, configArray));
}


CppConfigDictionary CppConfigFile::getConfigFromArray(std::string_view motherKey, const std::vector<std::string>& valueArray) {
	std::string key, value;
	CppConfigDictionary returnConfigDictionary(motherKey);
	int numBra = 0;
	for ( int lineNum = 0; lineNum < valueArray.size(); lineNum++ ) {
		std::string line = valueArray[lineNum];
		if ( (line.find('=') != std::string::npos && line.find('{') == std::string::npos) && numBra == 0 ) {
			line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, '\t'), line.begin() + line.find('=') + 2);
			line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, ' '), line.begin() + line.find('=') + 2);

			key = line.substr(0, line.find('='));
			value = line.substr(line.find('=') + 1);
			while ( true ) {
				if ( value[0] == '\t' || value[0] == ' ' ) {
					value.erase(value.begin());
				} else {
					break;
				}
			}
			returnConfigDictionary.addDictionary(key, value);
			key = "";
			value = "";
		} else if ( (line.find('=') != std::string::npos) && (line.find('{') != std::string::npos) && (line[line.find('{') - 1] != '\\') ) {
			line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, '\t'), line.begin() + line.find('=') + 2);
			line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, ' '), line.begin() + line.find('=') + 2);
			key = line.substr(0, line.find('='));
			std::vector<std::string> tempValueArray;
			int numSubBra = 0;
			lineNum++;
			while ( true ) {
				line = valueArray[lineNum];
				if ( (line.find('{') != std::string::npos) && (line[line.find('{') - 1] != '\\') ) {
					line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, '\t'), line.begin() + line.find('=') + 2);
					line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, ' '), line.begin() + line.find('=') + 2);
					tempValueArray.push_back(line);
					numSubBra++;
				} else if ( (line.find('}') != std::string::npos) && (line[line.find('}') - 1] != '\\') ) {
					if ( numSubBra != 0 ) {
						line.erase(remove(line.begin(), line.end(), '\t'), line.end());
						line.erase(remove(line.begin(), line.end(), ' '), line.end());
						tempValueArray.push_back(line);
					} else {
						returnConfigDictionary.addSubConfigDictionary(getConfigFromArray(key, tempValueArray));
						break;
					}
					numSubBra--;
				} else {
					line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, '\t'), line.begin() + line.find('=') + 2);
					line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, ' '), line.begin() + line.find('=') + 2);
					tempValueArray.push_back(line);
				}
				lineNum++;
			}
		} else if ( (line.find('=') != std::string::npos) && (line.find('{') != std::string::npos) && (line[line.find('{') - 1] == '\\') ) {
			line.erase(remove(line.begin(), line.end(), '\\'), line.end());
			line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, '\t'), line.begin() + line.find('=') + 2);
			line.erase(remove(line.begin(), line.begin() + line.find('=') + 2, ' '), line.begin() + line.find('=') + 2);
			key = line.substr(0, line.find('='));
			value = line.substr(line.find('=') + 1);
			while ( true ) {
				if ( value[0] == '\t' || value[0] == ' ' ) {
					value.erase(value.begin());
				} else {
					break;
				}
			}
			returnConfigDictionary.addDictionary(key, value);
			key = "";
			value = "";
		}
	}
	return returnConfigDictionary;
}

const std::vector<std::string> CppConfigFile::getConfigurableNameList() const {
	std::vector<std::string> configurableNameList;
	for ( const CppConfigDictionary& config : mConfigs ) {
		configurableNameList.push_back(std::string(config.getConfigName()));
	}
	return configurableNameList;
}


const CppConfigDictionary CppConfigFile::getConfig(std::string_view configTitle) const {
	CppConfigDictionary rConfig;
	for ( auto& config : mConfigs ) {
		if ( config.getConfigName() == configTitle ) {
			rConfig = config;
			break;
		}
	}
	return rConfig;
}

const bool CppConfigFile::hasConfig(std::string_view configTitle) const {
	bool hasValue = false;
	for ( auto& config : mConfigs ) {
		if ( config.getConfigName() == configTitle ) {
			hasValue = true;
		}
	}
	return hasValue;
}

std::ostream& operator<<(std::ostream& os, const CppConfigFile& copy) {
	std::cout << "Config list (white: keys, green: subConfig)" << std::endl;
	for ( auto& config : copy.mConfigs ) {
		os << config;
	}
	return os;
}

CppConfigFile::~CppConfigFile() { }