#include "cppUnit.h"

Unit::Unit() {}

Unit::Unit(std::string_view unit) {
    setUnit(unit);
}

std::vector<std::tuple<std::string, std::string, Unit>> Quantity::userQuantity = {};
std::vector<std::pair<std::string, std::array<int,7>>> Unit::userUnits = {};
std::vector<std::string> Unit::exceptPrefixList = {"m", "mol", "cd"};
bool Unit::isUserUnit = false;

void Unit::setUserUnit(std::string_view newUnit, std::string_view newSiUnit) {
    Unit object;
    std::vector<std::string> store;
    object.seperate(store, newSiUnit);
    object.vanishSlash(store);
    object.removePrefix(store);

    int iNow = 0;
    for (std::string& str : store) {
        if (isalpha(str[0])) {
            object.setUnitCount(store[iNow], stoi(store[iNow+1])); 
        }
        iNow++;
    }
    userUnits.push_back({std::string(newUnit), object.getUnitCount()});
    if (prefix.count(newUnit[0])) {
        exceptPrefixList.push_back(std::string(newUnit));
    }
    isUserUnit = true;

}

void Quantity::setUserQuantity() {
    std::ifstream unitFile("UserUnit.txt");
    std::string str;

    while(getline(unitFile, str)) {
        std::istringstream strstr(str);
        std::string newUnit, relation, newSiUnit;
        strstr >> newUnit >> relation >> newSiUnit;
        Unit::setUserUnit(newUnit, newSiUnit);
        Quantity::userQuantity.push_back(std::make_tuple(newUnit, relation, Unit(newSiUnit)));
    }
    unitFile.close();
}

void Unit::setUnit(std::string_view unit) {
    std::vector<std::string> store;

    if (seperate(store, unit) || vanishSlash(store) || removePrefix(store)) {}
    
    int iNow = 0;
    for (std::string& str : store) {
        if (isalpha(str[0])) {
            setUnitCount(store[iNow], stoi(store[iNow+1])); 
        }
        iNow++;
    }
}

bool Unit::seperate(std::vector<std::string>& store, std::string_view unit) {
    int iStart = 0;
    int iNow = 0;

    bool doubleSlash = false;
    bool isNum = false;

    bool error = false;
    for (const char letter : unit) {
        if (letter == '*') {
            store.push_back(static_cast<std::string>(unit.substr(iStart, iNow - iStart)));
            store.push_back(static_cast<std::string>(unit.substr(iNow,1)));
            iStart = iNow + 1;
        } else if (letter == '/') {
            if (doubleSlash) {
                std::cerr << "Unit notation to write '/' twice is forbidden" << std::endl;
                error = true;
            }
            store.push_back(static_cast<std::string>(unit.substr(iStart, iNow - iStart)));
            store.push_back(static_cast<std::string>(unit.substr(iNow,1)));
            iStart = iNow + 1;
            doubleSlash = true;
        } else if ((letter == '-' || isdigit(letter)) && !isNum) {
            store.push_back(static_cast<std::string>(unit.substr(iStart, iNow - iStart)));
            isNum = true;
            iStart = iNow;
        } else if (isNum && !isdigit(letter)) {
            iStart = iNow;
            isNum = false;
        }
        if (iNow == unit.size() - 1 ) {
            store.push_back(static_cast<std::string>(unit.substr(iStart)));
        }
        iNow++;
    }
    return error;
}

bool Unit::vanishSlash(std::vector<std::string>& store) {
    int iNow = 0;
    bool afterSlash = false;
    while (store.end() - store.begin() - iNow > 0 ) {
        std::string str = store[iNow];
        if (str == "/") {
            afterSlash = true;
            store[iNow] = "*";
        }
        if ((str[0] == '-' || isdigit(str[0])) && afterSlash) {
            store[iNow] = "-" + store[iNow];
        }
        if (isalpha(str[0]) && ((store[iNow+1][0] != '-' && !isdigit(store[iNow+1][0])) || (store.begin() + iNow + 1) == store.end())) {
            store.insert(store.begin() + iNow + 1, "1");
        }
        iNow++;
    }
    return false;
}

bool Unit::removePrefix(std::vector<std::string>& store) {
    mDigit = 0;

    int iNow = 0;
    for (std::string_view str : store) {
        if (prefix.count(str[0])) {
            if (std::find(exceptPrefixList.begin(), exceptPrefixList.end(), str) != exceptPrefixList.end()) {
            } else if (str.substr(1) == "g") {
                mDigit += (prefix.find(str[0])->second - 3) * stoi(store[iNow+1]);
                store[iNow] = "kg";
            } else {
                mDigit += prefix.find(str[0])->second * stoi(store[iNow+1]);
                store[iNow] = str.substr(1);
            }
        } else if (isalpha(str[0])){
        }
        iNow++;
    }
    return false;
}

void Unit::setUnitCount(std::string unit, int power) {
    if (unit == "m") {
        plusCount({1 * power, 0 * power, 0 * power, 0 * power, 0 * power, 0 * power, 0 * power});
    } else if (unit == "kg") {
        plusCount({0 * power, 1 * power, 0 * power, 0 * power, 0 * power, 0 * power, 0 * power});
    } else if (unit == "s") {
        plusCount({0 * power, 0 * power, 1 * power, 0 * power, 0 * power, 0 * power, 0 * power});
    } else if (unit == "A") {
        plusCount({0 * power, 0 * power, 0 * power, 1 * power, 0 * power, 0 * power, 0 * power});
    } else if (unit == "K") {
        plusCount({0 * power, 0 * power, 0 * power, 0 * power, 1 * power, 0 * power, 0 * power});
    } else if (unit == "mol") {
        plusCount({0 * power, 0 * power, 0 * power, 0 * power, 0 * power, 1 * power, 0 * power});
    } else if (unit == "cd") {
        plusCount({0 * power, 0 * power, 0 * power, 0 * power, 0 * power, 0 * power, 1 * power});        
    } else if (isUserUnit) {
        for (const std::pair<std::string, std::array<int,7>>& userUnit : userUnits) {
            if (userUnit.first == unit) {
                plusCount({userUnit.second[0] * power, userUnit.second[1] * power, userUnit.second[2] * power, userUnit.second[3] * power, userUnit.second[4] * power, userUnit.second[5] * power, userUnit.second[6] * power});
            }
        }
    } else {
        std::cerr << "'" << unit << "; is un-defined unit. Set user difine option." << std::endl;
    }
}

void Unit::plusCount(std::array<int,7> nums) {
    unitCount[0] += nums[0];
    unitCount[1] += nums[1];
    unitCount[2] += nums[2];
    unitCount[3] += nums[3];
    unitCount[4] += nums[4];
    unitCount[5] += nums[5];
    unitCount[6] += nums[6];
}

bool Unit::operator==(const Unit &ref) const {
    return ref.unitCount == unitCount;
}

bool Unit::operator!=(const Unit &ref) const {
    return ref.unitCount != unitCount;
}

Unit Unit::operator*(const Unit& ref) const {
    Unit result = *this;
    result *= ref;
    return result;
}

Unit Unit::operator*=(const Unit& ref) {
    mDigit += ref.mDigit;
    for (int i = 0; i < 7; i++) {
        unitCount[i] += ref.unitCount[i];
    }
    for (std::string_view nonBasic : ref.nonBasicUnit) {
        if (find(nonBasicUnit.begin(), nonBasicUnit.end(), nonBasic) != nonBasicUnit.end()) {
            nonBasicUnit.push_back(static_cast<std::string>(nonBasic));
        }
    }
    return *this;
}

Unit Unit::operator/(const Unit& ref) const {
    Unit result = *this;
    result /= ref;
    return result;
}

Unit Unit::operator/=(const Unit& ref) {
    mDigit -= ref.mDigit;
    for (int i = 0; i < 7; i++) {
        unitCount[i] -= ref.unitCount[i];
    }
    for (std::string_view nonBasic : ref.nonBasicUnit) {
        if (find(nonBasicUnit.begin(), nonBasicUnit.end(), nonBasic) != nonBasicUnit.end()) {
            nonBasicUnit.push_back(static_cast<std::string>(nonBasic));
        }
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, Unit& ref) {
    std::string unitTable[7] = {"m", "kg", "s", "A", "K", "mol", "cd"};
    int multiCount = 7 - std::count(std::begin(ref.unitCount), std::end(ref.unitCount), (int) 0);
    for (int i = 0; i < 7; i++) {
        if (ref.unitCount[i] != 0) {
            os << unitTable[i] << (int) ref.unitCount[i];
            multiCount--;
            if (multiCount > 0) {
                os << "*";
            }
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Unit& ref) {
    std::string unitTable[7] = {"m", "kg", "s", "A", "K", "mol", "cd"};
    int multiCount = 7 - std::count(std::begin(ref.unitCount), std::end(ref.unitCount), (int) 0);
    for (int i = 0; i < 7; i++) {
        if (ref.unitCount[i] != 0) {
            os << unitTable[i] << (int) ref.unitCount[i];
            multiCount--;
            if (multiCount > 0) {
                os << "*";
            }
        }
    }
    return os;
}

const int Unit::getDigit() const {
    return mDigit;
}

const std::array<int, 7>& Unit::getUnitCount() const {
    return unitCount;
}

const std::vector<std::string> Unit::getNonBasicUnits() const {
    return nonBasicUnit;
}

const std::string Unit::getUnit() const {
    std::string unitTable[7] = {"m", "kg", "s", "A", "K", "mol", "cd"};
    int multiCount = 7 - std::count(std::begin(unitCount), std::end(unitCount), (int) 0);
    std::string result;
    for (int i = 0; i < 7; i++) {
        if (unitCount[i] != 0) {
            result += unitTable[i] + std::to_string((int)unitCount[i]);
            multiCount--;
            if (multiCount > 0) {
                result += "*";
            }
        }
    }
    return result;
}

Quantity::Quantity(std::string quantity) {
    int posChar = 0;
    for (const uint letter : quantity ) {
        if (isdigit(letter) || (letter == '.') ) {
            posChar++;
        } else {
            break;
        }
    }
    mNum = stof(quantity.substr(0,posChar));
    mDigit = floor(log10(mNum));
    mNum = mNum * pow(10, -mDigit);

    mUnit = Unit(quantity.substr(posChar));
    mDigit += mUnit.getDigit();
}

Quantity::Quantity(double num, std::string unit) {
    mDigit = floor(log10(mDigit));
    mNum = mNum * pow(10, -mDigit);
    mUnit = Unit(unit);
}

double Quantity::getNum() const {
    return mNum * pow(10, mDigit);
}

double Quantity::getNum(std::string_view unit) const {
    Unit givenUnit(unit);
    if (mUnit != givenUnit) {
        std::cerr << "The operands doesn't have same dimension." << mUnit << "!=" << givenUnit << std::endl;
    }
    return mNum * pow(10, mDigit - givenUnit.getDigit());
}

const std::string Quantity::getUnit() const {
    return mUnit.getUnit();
}

const std::string Quantity::getQuantity() const {
    return std::to_string(mNum * pow(10, mDigit));
}

const std::string Quantity::getQuantity(std::string_view unit) const {
    Unit givenUnit(unit);
    if (mUnit != givenUnit) {
        std::cerr << "The operands doesn't have same dimension." << mUnit << "!=" << givenUnit << std::endl;
    }
    return std::to_string(mNum * pow(10, mDigit - givenUnit.getDigit())) + static_cast<std::string>(unit);
}

Quantity Quantity::operator+(const Quantity& ref) const {
    Quantity result = *this;
    result += ref;
    return result;
}
Quantity Quantity::operator-(const Quantity& ref) const {
    Quantity result = *this;
    result -= ref;
    return result;
}
Quantity Quantity::operator*(const Quantity& ref) const {
    Quantity result = *this;
    result *= ref;
    return result;
}
Quantity Quantity::operator/(const Quantity& ref) const {
    Quantity result = *this;
    result /= ref;
    return result;
}
Quantity Quantity::operator+=(const Quantity& ref) {
    if (mUnit != ref.mUnit) {
        std::cerr << "The operands doesn't have same dimension." << mUnit << "!=" << ref.mUnit << std::endl;
    } else {
        int mainDigit = std::max(mDigit, ref.mDigit);
        mNum = mNum * pow(10, mDigit - mainDigit) + ref.mNum * pow(10, ref.mDigit - mainDigit);
        mDigit = mainDigit + floor(log10(mNum));
        mNum = mNum * pow(10, -floor(log10(mNum)));
    }
    return *this;
}
Quantity Quantity::operator-=(const Quantity& ref) {
    if (mUnit != ref.mUnit) {
        std::cerr << "The operands doesn't have same dimension." << mUnit << "!=" << ref.mUnit << std::endl;
    } else {
        int mainDigit = std::max(mDigit, ref.mDigit);
        mNum = mNum * pow(10, mDigit - mainDigit) - ref.mNum * pow(10, ref.mDigit - mainDigit);
        mDigit = mainDigit + floor(log10(abs(mNum)));
        mNum = mNum * pow(10, -floor(log10(abs(mNum))));
    }
    return *this;
}
Quantity Quantity::operator*=(const Quantity& ref) {
    int mainDigit = std::max(mDigit, ref.mDigit);
    mNum = mNum * pow(10, mDigit - mainDigit) * ref.mNum * pow(10, ref.mDigit - mainDigit);
    mDigit = mainDigit + floor(log10(abs(mNum)));
    mNum = mNum * pow(10, -floor(log10(abs(mNum))));
    mUnit *= ref.mUnit;
    return *this;
}
Quantity Quantity::operator/=(const Quantity& ref) {
    int mainDigit = std::max(mDigit, ref.mDigit);
    mNum = mNum * pow(10, mDigit - mainDigit) / (ref.mNum * pow(10, ref.mDigit - mainDigit));
    mDigit = mainDigit + floor(log10(abs(mNum)));
    mNum = mNum * pow(10, -floor(log10(abs(mNum))));
    mUnit /= ref.mUnit;
    return *this;
}

std::ostream& operator<<(std::ostream& os, Quantity& ref) {
    os << ref.mNum << "e" << ref.mDigit << " " << ref.mUnit;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Quantity& ref) {
    os << ref.mNum << "e" << ref.mDigit << " " << ref.mUnit;
    return os;
}