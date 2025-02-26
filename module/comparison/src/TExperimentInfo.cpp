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

void TExperimentInfo::setPhi(int phi) {
	mPhi = phi;
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

void TExperimentInfo::setEntry(const std::array<double, 3>& entry) {
	mEntry = entry;
}

void TExperimentInfo::setError(const std::array<double, 3>& error) {
	mError = error;
}

const std::string& TExperimentInfo::getTag() const {
	return mTag;
}

int TExperimentInfo::getLength() const {
	return mLength;
}

int TExperimentInfo::getPhi() const {
	return mPhi;
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

const std::array<double, 2> TExperimentInfo::getCS1() const {
	std::array<double, 2> cs1;
	cs1[0] = mEntry[0];
	cs1[1] = mError[0];
	return cs1;
}

const std::array<double, 2> TExperimentInfo::getCS4TO32() const {
	std::array<double, 2> cs4to32;
	cs4to32[0] = mEntry[1];
	cs4to32[1] = mError[1];
	return cs4to32;
}

const std::array<double, 2> TExperimentInfo::getCS33Over() const {
	std::array<double, 2> cs33over;
	cs33over[0] = mEntry[2];
	cs33over[1] = mError[2];
	return cs33over;
}