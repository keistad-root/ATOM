#ifndef __CPPCONFIGERROR__
#define __CPPCONFIGERROR__

#include<exception>
#include<string>

class CppConfigFileError : public std::exception {
public:
	std::string mMessage;
public:
	CppConfigFileError(std::string_view errorType, std::string_view parameter) {
		if ( errorType == "FileExist" ) {
			mMessage = "The Config File \033[1;32m" + std::string(parameter) + "\033[1;0m doesn't exist ";
		} else if ( errorType == "ConfigDictionaryExist" ) {
			mMessage = "The Config Dictionary \033[1;32m" + std::string(parameter) + "\033[1;0m doesn't exist";
		}
	}
	const char* what() const throw() {
		return mMessage.c_str();
	}
};


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

#endif