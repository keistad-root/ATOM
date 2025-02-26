#include "TExperimentInfoSet.h"

#include<iostream>
#include<csv.h>

#include "config.h"
const std::string csvPath = std::string(SOURCE_DIR) + "/build/Data/EXPERIMENT.csv";

TExperimentInfoSet::TExperimentInfoSet() {
	io::CSVReader<12> expCSV(csvPath);
	expCSV.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "MINUTE", "CS1", "CS1_ERROR", "CS4TO32", "CS4TO32_ERROR", "CS33OVER", "CS33OVER_ERROR");
	std::string tag;
	int length, phi, minute;
	double collimatorLength, collimatorArea;
	std::array<double, 3> entry;
	std::array<double, 3> entryError;
	while ( expCSV.read_row(tag, length, phi, collimatorLength, collimatorArea, minute, entry[0], entryError[0], entry[1], entryError[1], entry[2], entryError[2]) ) {
		TExperimentInfo expInfo;
		expInfo.setTag(tag);
		expInfo.setLength(length);
		expInfo.setPhi(phi);
		expInfo.setCollimatorLength(collimatorLength);
		expInfo.setCollimatorArea(collimatorArea);
		expInfo.setMinute(minute);
		expInfo.setEntry(entry);
		expInfo.setError(entryError);
		mExperimentSet.push_back(expInfo);
	}
}


const std::vector<TExperimentInfo>& TExperimentInfoSet::getExperimentSet() const {
	return mExperimentSet;
}

const TExperimentInfo& TExperimentInfoSet::getExperimentInfo(const std::string& tag) const {
	for ( auto& exp : mExperimentSet ) {
		if ( exp.getTag() == tag ) {
			return exp;
		}
	}
	std::cerr << "No experiment data with tag " << tag << std::endl;
	exit(1);
}
