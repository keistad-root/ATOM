#ifndef __CPPCONFIG__
#define __CPPCONFIG__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include<filesystem>
#include<algorithm>
#include<fstream>

#include "CppConfigDictionary.h"
#include "CppConfigError.h"

class CppConfigFile {
private:
	std::vector<CppConfigDictionary> mConfigs;
public:
	CppConfigFile();
	CppConfigFile(std::string_view configFile);
	CppConfigFile(const CppConfigFile& copy);
	CppConfigFile& operator=(const CppConfigFile& copy);
	CppConfigFile(CppConfigFile&& move);
	CppConfigFile& operator=(CppConfigFile&& move);
	~CppConfigFile();
	void addConfig(std::string_view configFile);
	CppConfigDictionary getConfigFromArray(std::string_view key, const std::vector<std::string>& valueArray);
	void addConfig(std::string_view configTitle, const std::vector<std::string>& configArray);
	const std::vector<std::string> getConfigurableNameList() const;
	const CppConfigDictionary getConfig(std::string_view configTitle) const;
	const bool hasConfig(std::string_view configTitle) const;
	friend std::ostream& operator<<(std::ostream& os, const CppConfigFile& copy);
};



#endif