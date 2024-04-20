#ifndef __CPPUNIT__
#define __CPPUNIT__

#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>

const std::unordered_map<char, int> prefix = {{'a', -18}, {'f', -15}, {'p', -12}, {'n', -9}, {'u', -6}, {'m', -3}, {'c', -2}, {'k', 3}, {'M', 6}, {'G', 9}, {'T', 12}, {'P', 15}};

class Unit {
private:
	int mDigit;
	std::array<int, 7> unitCount = {0, 0, 0, 0, 0, 0, 0}; // 0: meter, 1: kilogram, 2: sec, 3: ampare, 4: kelvin, 5: mol, 6: candela
	std::vector<std::string> nonBasicUnit;
	static std::vector<std::pair<std::string, std::array<int, 7>>> userUnits;
	static std::vector<std::string> exceptPrefixList;
	static bool isUserUnit;
public:
	Unit();
	Unit(std::string_view unit);

	static void setUserUnit(std::string_view newUnit, std::string_view newSiUnit);
	void setUnit(std::string_view unit);

	bool operator==(const Unit& ref) const;
	bool operator!=(const Unit& ref) const;
	Unit operator*(const Unit& ref) const;
	Unit operator*=(const Unit& ref);
	Unit operator/(const Unit& ref) const;
	Unit operator/=(const Unit& ref);
	friend std::ostream& operator<<(std::ostream& os, Unit& ref);
	friend std::ostream& operator<<(std::ostream& os, const Unit& ref);

	const int getDigit() const;
	const std::array<int, 7>& getUnitCount() const;
	const std::vector<std::string> getNonBasicUnits() const;
	const std::string getUnit() const;
private:
	bool seperate(std::vector<std::string>& store, std::string_view unit);
	bool vanishSlash(std::vector<std::string>& store);
	bool removePrefix(std::vector<std::string>& store);
	void setUnitCount(std::string unit, int power);
	void plusCount(std::array<int, 7> nums);
};

class Quantity {
private:
	double mNum;
	int mDigit;
	Unit mUnit;
	static std::vector<std::tuple<std::string, std::string, Unit>> userQuantity;
public:
	static void setUserQuantity(); // There are user unit txt file should be located in same directory.
	Quantity() = delete;
	Quantity(std::string quantity);
	Quantity(double num, std::string unit);

	double getNum() const;
	double getNum(std::string_view unit) const;
	const std::string getUnit() const;
	const std::string getQuantity() const;
	const std::string getQuantity(std::string_view unit) const;

	Quantity operator+(const Quantity& ref) const;
	Quantity operator-(const Quantity& ref) const;
	Quantity operator*(const Quantity& ref) const;
	Quantity operator/(const Quantity& ref) const;
	Quantity operator+=(const Quantity& ref);
	Quantity operator-=(const Quantity& ref);
	Quantity operator*=(const Quantity& ref);
	Quantity operator/=(const Quantity& ref);

	friend std::ostream& operator<<(std::ostream& os, Quantity& ref);
	friend std::ostream& operator<<(std::ostream& os, const Quantity& ref);
};

#endif