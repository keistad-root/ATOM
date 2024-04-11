#ifndef __CPPCONFIG__
#define __CPPCONFIG__

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

class Configurable {
private:
	std::string mConfigName;
	std::unordered_map<std::string, std::string> mDictionary;

public:
	Configurable(std::string_view configName);
	~Configurable();
	void addDictionary(std::string_view key, std::string_view value);
	const std::string& getConfigName() const;
	const std::string& find(std::string_view key) const;
	const std::unordered_map<std::string, std::string>& getDictionary() const;

	friend std::ostream& operator<<(std::ostream& os, const Configurable& copy);

};

class Configuration {
private:
	std::vector<Configurable> mConfigs;
public:
	Configuration(std::string_view configFile);
	~Configuration();
	void addConfig(std::string_view configFile);
	const std::vector<std::string> getConfigurableNameList() const;
	std::optional<Configurable> getConfig(std::string_view configTitle) const;
	friend std::ostream& operator<<(std::ostream& os, const Configuration& copy);
};


#endif