#include "TGeantInfo.h"

TGeantInfo::TGeantInfo() { }
TGeantInfo::~TGeantInfo() { }

void TGeantInfo::setTag(const std::string& tag) {
	mTag = tag;
}

void TGeantInfo::setLength(int length) {
	mLength = length;
}

void TGeantInfo::setPhi(int phi) {
	mPhi = phi;
}

void TGeantInfo::setCollimatorLength(double collimatorLength) {
	mCollimatorLength = collimatorLength;
}

void TGeantInfo::setCollimatorArea(double collimatorArea) {
	mCollimatorArea = collimatorArea;
}

void TGeantInfo::setDistanceAlpideCollimator(double distanceAlpideCollimator) {
	mDistanceAlpideCollimator = distanceAlpideCollimator;
}

void TGeantInfo::setDistanceSourceCollimator(double distanceSourceCollimator) {
	mDistanceSourceCollimator = distanceSourceCollimator;
}

void TGeantInfo::setAlpidePositionX(double alpidePositionX) {
	mAlpidePositionX = alpidePositionX;
}

void TGeantInfo::setAlpidePositionY(double alpidePositionY) {
	mAlpidePositionY = alpidePositionY;
}

void TGeantInfo::setAlpideAngleX(double alpideAngleX) {
	mAlpideAngleX = alpideAngleX;
}

void TGeantInfo::setAlpideAngleY(double alpideAngleY) {
	mAlpideAngleY = alpideAngleY;
}

void TGeantInfo::setAlpideAngleZ(double alpideAngleZ) {
	mAlpideAngleZ = alpideAngleZ;
}

void TGeantInfo::setNEvent(int nEvent) {
	mNEvent = nEvent;
}

void TGeantInfo::setEntry(const std::array<double, 4>& entry) {
	mEntry = entry;
}

void TGeantInfo::setError(const std::array<double, 4>& error) {
	mError = error;
}

const std::string& TGeantInfo::getTag() const {
	return mTag;
}

int TGeantInfo::getLength() const {
	return mLength;
}

int TGeantInfo::getPhi() const {
	return mPhi;
}

double TGeantInfo::getCollimatorLength() const {
	return mCollimatorLength;
}

double TGeantInfo::getCollimatorArea() const {
	return mCollimatorArea;
}

int TGeantInfo::getNEvent() const {
	return mNEvent;
}

double TGeantInfo::getDistanceAlpideCollimator() const {
	return mDistanceAlpideCollimator;
}

double TGeantInfo::getDistanceSourceCollimator() const {
	return mDistanceSourceCollimator;
}

double TGeantInfo::getAlpidePositionX() const {
	return mAlpidePositionX;
}

double TGeantInfo::getAlpidePositionY() const {
	return mAlpidePositionY;
}

double TGeantInfo::getAlpideAngleX() const {
	return mAlpideAngleX;
}

double TGeantInfo::getAlpideAngleY() const {
	return mAlpideAngleY;
}

double TGeantInfo::getAlpideAngleZ() const {
	return mAlpideAngleZ;
}

const std::array<double, 2> TGeantInfo::getEEM() const {
	return {mEntry[0], mError[0]};
}

const std::array<double, 2> TGeantInfo::getEAM() const {
	return {mEntry[1], mError[1]};
}

const std::array<double, 2> TGeantInfo::getEAE() const {
	return {mEntry[2], mError[2]};
}

const std::array<double, 2> TGeantInfo::getDouble() const {
	return {mEntry[3], mError[3]};
}

