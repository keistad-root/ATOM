#include "CppConfigDictionary.h"

CppConfigDictionary::CppConfigDictionary() = default;

CppConfigDictionary::CppConfigDictionary(std::string_view configName) : mConfigName(configName) { }

CppConfigDictionary::CppConfigDictionary(const CppConfigDictionary& copy) : mConfigName(copy.mConfigName), mDictionary(copy.mDictionary), mSubConfigDictionary(copy.mSubConfigDictionary) {

}
CppConfigDictionary& CppConfigDictionary::operator=(const CppConfigDictionary& copy) {
	mConfigName = copy.mConfigName;
	mDictionary = copy.mDictionary;
	mSubConfigDictionary = copy.mSubConfigDictionary;

	return *this;
}
CppConfigDictionary::CppConfigDictionary(CppConfigDictionary&& move) : mConfigName(move.mConfigName), mDictionary(move.mDictionary), mSubConfigDictionary(move.mSubConfigDictionary) {

}
CppConfigDictionary& CppConfigDictionary::operator=(CppConfigDictionary&& move) {
	mConfigName = move.mConfigName;
	mDictionary = move.mDictionary;
	mSubConfigDictionary = move.mSubConfigDictionary;

	mDictionary.clear();
	mSubConfigDictionary.clear();

	return *this;
}


void CppConfigDictionary::addDictionary(std::string_view key, std::string_view value) {
	mDictionary.insert_or_assign(std::string(key), std::string(value));
}

void CppConfigDictionary::addSubConfigDictionary(const CppConfigDictionary& subConfigDictionary) {
	mSubConfigDictionary.push_back(subConfigDictionary);
}

bool CppConfigDictionary::hasKey(std::string_view key) const {
	for ( const auto& dict : mSubConfigDictionary ) {
		if ( dict.getConfigName() == key ) {
			return true;
		}
	}
	if ( mDictionary.count(std::string(key)) ) {
		return true;
	} else {
		return false;
	}
}

const std::string& CppConfigDictionary::find(const std::string& key) const {
	if ( key.find('/') == std::string::npos ) {
		if ( mDictionary.count(key) == 1 ) {
			return mDictionary.find(key)->second;
		} else {
			return "";
		}
	} else {
		if ( hasKey(key.substr(0, key.find('/')).substr(0, key.find('/'))) ) {
			return getSubConfig(key.substr(0, key.find('/'))).find(key.substr(key.find('/') + 1));
		} else {
			return "";
		}
	}
}

std::string_view CppConfigDictionary::getConfigName() const {
	return mConfigName;
}

std::ostream& operator<<(std::ostream& os, const CppConfigDictionary& copy) {
	if ( copy.getConfigName().length() < 8 ) {
		os << copy.getConfigName() << "\t\t";
	} else if ( copy.getConfigName().length() < 16 ) {
		os << copy.getConfigName() << "\t";
	} else {
		os << copy.getConfigName() << std::endl << "\t\t";
	}
	for ( auto& dict : copy.mDictionary ) {
		os << "\033[1;47m" << dict.first << "\033[1;0m" << ", ";
	}
	for ( auto& subConfig : copy.mSubConfigDictionary ) {
		os << "\033[1;42m" << subConfig.getConfigName() << "\033[1;0m" << ", ";
	}
	os << std::endl;
	return os;
}

const std::vector<std::string> CppConfigDictionary::getKeyList() const {
	std::vector<std::string> returnArray;

	for ( const auto& pair : mDictionary ) {
		returnArray.push_back(pair.first);
	}

	for ( const CppConfigDictionary& subConfigDictionary : mSubConfigDictionary ) {
		for ( std::string_view subConfigKey : subConfigDictionary.getKeyList() ) {
			returnArray.push_back(std::string(subConfigDictionary.getConfigName()) + "/" + std::string(subConfigKey));
		}
	}
	return returnArray;
}

std::unordered_map<std::string, std::string> CppConfigDictionary::getDictionary() {
	return mDictionary;
}

const CppConfigDictionary& CppConfigDictionary::getSubConfig(std::string_view key) const {
	try {
		bool isExist = false;
		for ( const CppConfigDictionary& subConfigDictionary : mSubConfigDictionary ) {
			if ( subConfigDictionary.getConfigName() == key ) {
				isExist = true;
				return subConfigDictionary;
			}
		}
		if ( !isExist ) { throw CppConfigFileError("ConfigDictionaryExist", key); }
	} catch ( CppConfigFileError error ) {
		std::cerr << error.what() << std::endl;
	}
}

const std::vector<std::string> CppConfigDictionary::getValueList() const {
	std::vector<std::string> returnArray;

	for ( const auto& pair : mDictionary ) {
		returnArray.push_back(pair.second);
	}

	return returnArray;
}

CppConfigDictionary& CppConfigDictionary::operator+(const CppConfigDictionary& copy) {
	for ( const auto& pair : copy.mDictionary ) {
		mDictionary.insert_or_assign(pair.first, pair.second);
	}
	for ( const CppConfigDictionary& solo : copy.mSubConfigDictionary ) {
		mSubConfigDictionary.push_back(solo);
	}

	return *this;
}

CppConfigDictionary& CppConfigDictionary::operator+=(const CppConfigDictionary& copy) {
	for ( const auto& pair : copy.mDictionary ) {
		mDictionary.insert_or_assign(pair.first, pair.second);
	}
	for ( const CppConfigDictionary& solo : copy.mSubConfigDictionary ) {
		mSubConfigDictionary.push_back(solo);
	}

	return *this;
}

const std::vector<CppConfigDictionary> CppConfigDictionary::getSubConfigSet() const {
	return mSubConfigDictionary;
}

const std::unordered_map<std::string, CppConfigDictionary> CppConfigDictionary::getSubConfigSetWithName() const {
	std::unordered_map<std::string, CppConfigDictionary> returnMap;
	for ( const CppConfigDictionary& configDictionary : mSubConfigDictionary ) {
		returnMap.insert_or_assign(std::string(configDictionary.getConfigName()), configDictionary);
	}
	return returnMap;
}
