#include "TGeantInfoSet.h"
#include<iostream>
#include<csv.h>

#include "config.h"
const std::string csvPath = std::string(SOURCE_DIR) + "/build/Data/g4data.csv";

TGeantInfoSet::TGeantInfoSet() {
	io::CSVReader<21> geantCSV(csvPath);
	geantCSV.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "EVENT_NUM", "DISTANCE_ALPIDE_COLLIMATOR", "DISTANCE_SOURCE_COLLIMATOR", "ALPIDE_POSITION_X", "ALPIDE_POSITION_Y", "ALPIDE_ANGLE_X", "ALPIDE_ANGLE_Y", "ALPIDE_ANGLE_Z", "EEM", "EEM_ERROR", "EAM", "EAM_ERROR", "EAE", "EAE_ERROR", "DOUBLE", "DOUBLE_ERROR");
	std::string tag;
	int length, phi, eventNum;
	double collimatorLength, collimatorArea, distanceAlpideCollimator, distanceSourceCollimator, alpidePositionX, alpidePositionY, alpideAngleX, alpideAngleY, alpideAngleZ;
	std::array<double, 4> entry;
	std::array<double, 4> error;

	while ( geantCSV.read_row(tag, length, phi, collimatorLength, collimatorArea, eventNum, distanceAlpideCollimator, distanceSourceCollimator, alpidePositionX, alpidePositionY, alpideAngleX, alpideAngleY, alpideAngleZ, entry[0], error[0], entry[1], error[1], entry[2], error[2], entry[3], error[3]) ) {
		TGeantInfo geantInfo;
		geantInfo.setTag(tag);
		geantInfo.setLength(length);
		geantInfo.setPhi(phi);
		geantInfo.setCollimatorLength(collimatorLength);
		geantInfo.setCollimatorArea(collimatorArea);
		geantInfo.setNEvent(eventNum);
		geantInfo.setDistanceAlpideCollimator(distanceAlpideCollimator);
		geantInfo.setDistanceSourceCollimator(distanceSourceCollimator);
		geantInfo.setAlpidePositionX(alpidePositionX);
		geantInfo.setAlpidePositionY(alpidePositionY);
		geantInfo.setAlpideAngleX(alpideAngleX);
		geantInfo.setAlpideAngleY(alpideAngleY);
		geantInfo.setAlpideAngleZ(alpideAngleZ);
		geantInfo.setEntry(entry);
		geantInfo.setError(error);
		mGeantSet.push_back(geantInfo);
	}
}

TGeantInfoSet::~TGeantInfoSet() { }

const std::vector<TGeantInfo>& TGeantInfoSet::getGeantSet() const {
	return mGeantSet;
}

const TGeantInfo& TGeantInfoSet::getGeantInfo(const std::string& tag) const {
	for ( auto& geant : mGeantSet ) {
		if ( geant.getTag() == tag ) {
			return geant;
		}
	}
	std::cerr << "No geant data with tag " << tag << std::endl;
	exit(1);
}