#ifndef __TCSV__
#define __TCSV__

#include<filesystem>
#include<fstream>
#include<sstream>
#include<bits/stdc++.h>

class TCSVLine {
public:
	TCSVLine(const std::string str);
private:
	std::vector<std::string> mValueSet;
public:
	const std::string& getValue(const int n) const;
	const std::vector<std::string>& getLine() const;
};

class TCSV {
public:
	TCSV() = delete;
	TCSV(std::filesystem::path path);
	~TCSV();

private:
	int nParameter = 0;
	int nLine = 0;

	std::vector<std::string> mTitleSet;
	std::vector<TCSVLine> mRowSet;

public:
	const std::vector<std::string>& getRow(const int n);
	const std::vector<std::string> getColumn(const int n);
	const int getColumnNumber(const std::string& title) const;
	const std::vector<std::string> getColumn(const std::string& title);

};

#endif