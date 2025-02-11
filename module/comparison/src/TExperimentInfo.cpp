#include "TExperimentInfo.h"
#include<cmath>
#include "TMathUser.h"
TExperimentInfo::TExperimentInfo() { }

TExperimentInfo::~TExperimentInfo() { }

void TExperimentInfo::setTag(const std::string& tag) {
	mTag = tag;
}

void TExperimentInfo::setLength(int length) {
	mLength = length;
}

void TExperimentInfo::setCollimatorLength(double collimatorLength) {
	mCollimatorLength = collimatorLength;
}

void TExperimentInfo::setCollimatorArea(double collimatorArea) {
	mCollimatorArea = collimatorArea;
}

void TExperimentInfo::setMinute(int minute) {
	mMinute = minute;
}

void TExperimentInfo::setEntry(const std::array<double, 60>& entry) {
	mEntry = entry;
}

void TExperimentInfo::setError(const std::array<double, 60>& error) {
	mError = error;
}

const std::string& TExperimentInfo::getTag() const {
	return mTag;
}

int TExperimentInfo::getLength() const {
	return mLength;
}

double TExperimentInfo::getCollimatorLength() const {
	return mCollimatorLength;
}

double TExperimentInfo::getCollimatorArea() const {
	return mCollimatorArea;
}

int TExperimentInfo::getMinute() const {
	return mMinute;
}

const std::array<double, 60>& TExperimentInfo::getEntry() const {
	return mEntry;
}

const std::array<double, 60>& TExperimentInfo::getError() const {
	return mError;
}

const std::array<double, 2> TExperimentInfo::getSubEntry(int start, int end) const {
	std::array<double, 2> subEntry = {0, 0};
	for ( int i = start - 1; i < end; i++ ) {
		subEntry[0] += mEntry[i];
		subEntry[1] += std::pow(mError[i], 2);
	}
	subEntry[1] = std::sqrt(subEntry[1]);
	return getEffectiveNumber(subEntry[0], subEntry[1]);
}