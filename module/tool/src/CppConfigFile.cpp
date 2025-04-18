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

void CppConfigFile::addConfig(CppConfigDictionary& config) {
	mConfigs.push_back(config);
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
// void CppConfigFile::addConfig(std::string_view configFile) {
// 	// Open config file
// 	std::ifstream conf{std::string(configFile)};

// 	// The reading line
// 	std::string line;
// 	// The key 
// 	std::string key;
// 	// The value is stored as std::string form
// 	std::string value;

// 	// Read config file line by line
// 	std::vector<std::string> valueArray;
// 	bool isFirst = true;
// 	while ( getline(conf, line) ) {
// 		// Remove comment out part
// 		std::string result;
// 		bool escapeNext = false;

// 		for ( char c : line ) {
// 			if ( escapeNext ) {
// 				if ( c == '#' ) {
// 					result += '#';
// 				} else {
// 					result += '\\';
// 					result += c;
// 				}
// 				escapeNext = false;
// 			} else {
// 				if ( c == '\\' ) {
// 					escapeNext = true;
// 				} else {
// 					result += c;
// 				}
// 			}
// 		}

// 		if ( escapeNext ) {
// 			result += '\\'; // Add trailing backslash if input ends with a backslash
// 		}
// 		line = result;
// 		line.erase(remove(line.begin(), line.end(), '\t'), line.end());
// 		if ( line[0] == '#' ) {
// 			line = "";
// 		}
// 		if ( line.find('=') != std::string::npos ) {
// 			line.erase(remove(line.begin(), line.begin() + line.find('='), ' '), line.begin() + line.find('='));
// 		} else {
// 			line.erase(remove(line.begin(), line.end(), ' '), line.end());
// 		}
// 		if ( line == "" ) { // Pass the blank line
// 			continue;
// 		} else if ( line.find('[') != std::string::npos && (line[line.find('[') - 1] != '\\') ) { // The line started with '[' is considered as Configurable name
// 			if ( !isFirst ) {
// 				mConfigs.push_back(getConfigFromArray(key, valueArray));
// 				valueArray.clear();
// 			} else {
// 				isFirst = false;
// 			}
// 			key = line.substr(line.find('[') + 1, line.find(']') - 1);
// 			// Make new configurable  
// 		} else {
// 			valueArray.push_back(line);
// 		}
// 	}
// 	if ( valueArray.size() != 0 ) {
// 		mConfigs.push_back(getConfigFromArray(key, valueArray));
// 	}
// }

void CppConfigFile::addConfig(std::string_view configTitle, const std::vector<std::string>& configArray) {
	mConfigs.push_back(getConfigFromArray(configTitle, configArray));
}

void CppConfigFile::addConfig(std::string_view configFile) {
	std::ifstream conf{std::string(configFile)};
	std::string line, key;
	std::vector<std::string> valueArray;
	bool isFirst = true;

	while ( getline(conf, line) ) {
		line = removeCommentsAndWhitespace(line);
		if ( line.empty() ) continue;

		if ( isConfigurableName(line) ) {
			if ( !isFirst ) {
				mConfigs.push_back(getConfigFromArray(key, valueArray));
				valueArray.clear();
			} else {
				isFirst = false;
			}
			key = extractConfigurableName(line);
		} else {
			valueArray.push_back(line);
		}
	}

	if ( !valueArray.empty() ) {
		mConfigs.push_back(getConfigFromArray(key, valueArray));
	}
}

std::string CppConfigFile::removeCommentsAndWhitespace(const std::string& line) {
	std::string result;
	bool escapeNext = false;
	bool inQuotes = false;

	for ( char c : line ) {
		if ( escapeNext ) {
			result += c;
			escapeNext = false;
		} else {
			if ( c == '\\' ) {
				escapeNext = true;
				result += c;
			} else if ( c == '"' ) {
				inQuotes = !inQuotes;
				result += c;
			} else if ( !inQuotes && c == '#' ) {
				break; // Ignore the rest of the line after a comment
			} else {
				result += c;
			}
		}
	}

	result.erase(remove(result.begin(), result.end(), '\t'), result.end());
	if ( !inQuotes && !result.empty() && result[0] == '#' ) {
		result.clear();
	}

	size_t equalPos = result.find('=');
	if ( equalPos != std::string::npos ) {
		result.erase(remove(result.begin(), result.begin() + equalPos, ' '), result.begin() + equalPos);
	} else {
		result.erase(remove(result.begin(), result.end(), ' '), result.end());
	}

	return result;
}

bool CppConfigFile::isConfigurableName(const std::string& line) {
	bool inQuotes = false;
	for ( char c : line ) {
		if ( c == '"' ) {
			inQuotes = !inQuotes;
		} else if ( c == '[' && !inQuotes ) {
			return true;
		}
	}
	return false;
}

std::string CppConfigFile::extractConfigurableName(const std::string& line) {
	size_t start = line.find('[') + 1;
	size_t end = line.find(']');
	return line.substr(start, end - start);
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

CppConfigDictionary& CppConfigFile::modifyConfig(std::string_view configTitle) {
	CppConfigDictionary* rConfig;
	for ( auto& config : mConfigs ) {
		if ( config.getConfigName() == configTitle ) {
			rConfig = &config;
			break;
		}
	}
	return *rConfig;
}

const CppConfigDictionary& CppConfigFile::getConfig(std::string_view configTitle) const {
	CppConfigDictionary* rConfig;
	for ( auto& config : mConfigs ) {
		if ( config.getConfigName() == configTitle ) {
			return config;
			// rConfig = &config;
			break;
		}
	}
	return *rConfig;
}

bool CppConfigFile::hasConfig(std::string_view configTitle) const {
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