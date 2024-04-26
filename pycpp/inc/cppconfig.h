#ifndef __CPPCONFIG__
#define __CPPCONFIG__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>

class ConfigrableNoValue : public std::exception {
public:
	std::string message;
public:
	ConfigrableNoValue(std::string_view key, std::string_view configName) {
		message = static_cast<std::string>(key) + " cannot be found in list of values of " + static_cast<std::string>(configName);
	}
	const char* what() const throw() {
		return message.c_str();
	}
};

class Configurable {
private:
	std::string mConfigName;
	std::unordered_map<std::string, std::string> mDictionary;

public:
	Configurable(std::string_view configName);
	Configurable(const Configurable& copy);
	~Configurable();
	void addDictionary(std::string_view key, std::string_view value);
	const std::string& getConfigName() const;
	const bool hasKey(std::string_view key) const;
	const std::string& find(std::string_view key) const;
	const std::vector<std::string> findlist(std::string_view key) const;
	const std::vector<std::string> getKeyList() const;
	std::unordered_map<std::string, std::string> getDictionary();

	friend std::ostream& operator<<(std::ostream& os, const Configurable& copy);

};

class Configuration {
private:
	std::vector<Configurable*> mConfigs;
public:
	Configuration() = default;
	Configuration(std::string_view configFile);
	~Configuration();
	void addConfig(std::string_view configFile);
	void addConfig(std::string_view configTitle, const std::vector<std::string>& configArray);
	const std::vector<std::string> getConfigurableNameList() const;
	const Configurable* getConfig(std::string_view configTitle) const;
	const bool hasConfig(std::string_view configTitle) const;
	friend std::ostream& operator<<(std::ostream& os, const Configuration& copy);
};


#endif