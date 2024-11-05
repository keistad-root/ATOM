#include "TCSV.h"

TCSVLine::TCSVLine(std::string str) {
	std::stringstream ss(str);

	std::string value;
	while ( std::getline(ss, value, ',') ) {
		if ( *value.begin() == ' ' ) {
			value.erase(0, 1);
		}
		if ( *value.end() == ' ' ) {
			value.erase(value.end());
		}
		mValueSet.push_back(value);
	}
}

const std::string& TCSVLine::getValue(const int n) const {
	return mValueSet[n];
}

const std::vector<std::string>& TCSVLine::getLine() const {
	return mValueSet;
}


TCSV::TCSV(std::filesystem::path path) {
	std::ifstream file(path);

	std::string line;

	std::getline(file, line);
	nParameter = std::count(line.begin(), line.end(), ',') + 1;

	std::stringstream ss(line);
	std::string title;

	while ( std::getline(ss, title, ',') ) {
		if ( *title.begin() == ' ' ) {
			title.erase(0, 1);
		}
		if ( *title.end() == ' ' ) {
			title.erase(title.end());
		}
		mTitleSet.push_back(title);
	}

	while ( std::getline(file, line) ) {
		mRowSet.push_back(TCSVLine(line));
		nLine++;
	}
}

TCSV::~TCSV() { }

const std::vector<std::string>& TCSV::getRow(const int n) {
	return mRowSet[n].getLine();
}

const std::vector<std::string> TCSV::getColumn(const int n) {
	std::vector<std::string> column;

	for ( int iLine = 0; iLine < nLine; iLine++ ) {
		column.push_back(mRowSet[iLine].getValue(n));
	}
	return column;
}

const int TCSV::getColumnNumber(const std::string& title) const {
	return std::find(mTitleSet.begin(), mTitleSet.end(), title) - mTitleSet.begin();
}

const std::vector<std::string> TCSV::getColumn(const std::string& title) {
	int num = getColumnNumber(title);
	return getColumn(num);
}