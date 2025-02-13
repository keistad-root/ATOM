#include "TGeantInfoSet.h"
#include<iostream>
#include<csv.h>

#include "config.h"
const std::string csvPath = std::string(SOURCE_DIR) + "/build/Data/g4data.csv";

TGeantInfoSet::TGeantInfoSet() {
	io::CSVReader<14> geantCSV(csvPath);
	geantCSV.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "EVENT_NUM", "EEM", "EEM_ERROR", "EAM", "EAM_ERROR", "EAE", "EAE_ERROR", "DOUBLE", "DOUBLE_ERROR");
	std::string tag;
	int length, phi, eventNum;
	double collimatorLength, collimatorArea;
	std::array<double, 4> entry;
	std::array<double, 4> error;

	while ( geantCSV.read_row(tag, length, phi, collimatorLength, collimatorArea, eventNum, entry[0], error[0], entry[1], error[1], entry[2], error[2], entry[3], error[3]) ) {
		TGeantInfo geantInfo;
		geantInfo.setTag(tag);
		geantInfo.setLength(length);
		geantInfo.setPhi(phi);
		geantInfo.setCollimatorLength(collimatorLength);
		geantInfo.setCollimatorArea(collimatorArea);
		geantInfo.setNEvent(eventNum);
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