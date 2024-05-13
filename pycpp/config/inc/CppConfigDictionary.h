#ifndef __CPPCONFIGDICTIONARY__
#define __CPPCONFIGDICTIONARY__

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "CppConfigError.h"

class CppConfigDictionary {
	std::string mConfigName;
	std::unordered_map<std::string, std::string> mDictionary;
	std::vector<CppConfigDictionary> mSubConfigDictionary;
public:
	CppConfigDictionary();
	CppConfigDictionary(const CppConfigDictionary& copy);
	CppConfigDictionary& operator=(const CppConfigDictionary& copy);
	CppConfigDictionary(CppConfigDictionary&& move);
	CppConfigDictionary& operator=(CppConfigDictionary&& move);

	CppConfigDictionary(std::string_view configName);
	void addDictionary(std::string_view key, std::string_view value);
	void addSubConfigDictionary(const CppConfigDictionary& subConfigDictionary);

	const bool hasKey(std::string_view key) const;
	const std::string& find(const std::string& key) const;
	const CppConfigDictionary& getSubConfig(std::string_view key) const;
	const std::vector<CppConfigDictionary> getSubConfigSet() const;
	const std::vector<std::string> getValueList() const;
	const std::vector<std::string> getKeyList() const;
	std::unordered_map<std::string, std::string> getDictionary();
	const std::unordered_map<std::string, CppConfigDictionary> getSubConfigSetWithName() const;

	std::string_view getConfigName() const;

	friend std::ostream& operator<<(std::ostream& os, const CppConfigDictionary& copy);
	CppConfigDictionary& operator+(const CppConfigDictionary& copy);
	CppConfigDictionary& operator+=(const CppConfigDictionary& copy);
};


#endif