#include "TExperimentInfoSet.h"

#include<iostream>
#include<csv.h>

#include "config.h"
const std::string csvPath = std::string(SOURCE_DIR) + "/build/Data/experiment_alpha.csv";

TExperimentInfoSet::TExperimentInfoSet() {
	io::CSVReader<126> expCSV(csvPath);
	expCSV.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "MINUTE", "CS1", "CS1_ERROR", "CS2", "CS2_ERROR", "CS3", "CS3_ERROR", "CS4", "CS4_ERROR", "CS5", "CS5_ERROR", "CS6", "CS6_ERROR", "CS7", "CS7_ERROR", "CS8", "CS8_ERROR", "CS9", "CS9_ERROR", "CS10", "CS10_ERROR", "CS11", "CS11_ERROR", "CS12", "CS12_ERROR", "CS13", "CS13_ERROR", "CS14", "CS14_ERROR", "CS15", "CS15_ERROR", "CS16", "CS16_ERROR", "CS17", "CS17_ERROR", "CS18", "CS18_ERROR", "CS19", "CS19_ERROR", "CS20", "CS20_ERROR", "CS21", "CS21_ERROR", "CS22", "CS22_ERROR", "CS23", "CS23_ERROR", "CS24", "CS24_ERROR", "CS25", "CS25_ERROR", "CS26", "CS26_ERROR", "CS27", "CS27_ERROR", "CS28", "CS28_ERROR", "CS29", "CS29_ERROR", "CS30", "CS30_ERROR", "CS31", "CS31_ERROR", "CS32", "CS32_ERROR", "CS33", "CS33_ERROR", "CS34", "CS34_ERROR", "CS35", "CS35_ERROR", "CS36", "CS36_ERROR", "CS37", "CS37_ERROR", "CS38", "CS38_ERROR", "CS39", "CS39_ERROR", "CS40", "CS40_ERROR", "CS41", "CS41_ERROR", "CS42", "CS42_ERROR", "CS43", "CS43_ERROR", "CS44", "CS44_ERROR", "CS45", "CS45_ERROR", "CS46", "CS46_ERROR", "CS47", "CS47_ERROR", "CS48", "CS48_ERROR", "CS49", "CS49_ERROR", "CS50", "CS50_ERROR", "CS51", "CS51_ERROR", "CS52", "CS52_ERROR", "CS53", "CS53_ERROR", "CS54", "CS54_ERROR", "CS55", "CS55_ERROR", "CS56", "CS56_ERROR", "CS57", "CS57_ERROR", "CS58", "CS58_ERROR", "CS59", "CS59_ERROR", "CS60", "CS60_ERROR");
	std::string tag;
	int length, phi, minute;
	double collimatorLength, collimatorArea;
	std::array<double, 60> entry;
	std::array<double, 60> entryError;
	while ( expCSV.read_row(tag, length, phi, collimatorLength, collimatorArea, minute, entry[0], entryError[0], entry[1], entryError[0], entry[2], entryError[2], entry[3], entryError[3], entry[4], entryError[4], entry[5], entryError[5], entry[6], entryError[6], entry[7], entryError[7], entry[8], entryError[8], entry[9], entryError[9], entry[10], entryError[10], entry[11], entryError[11], entry[12], entryError[12], entry[13], entryError[13], entry[14], entryError[14], entry[15], entryError[15], entry[16], entryError[16], entry[17], entryError[17], entry[18], entryError[18], entry[19], entryError[19], entry[20], entryError[20], entry[21], entryError[21], entry[22], entryError[22], entry[23], entryError[23], entry[24], entryError[24], entry[25], entryError[25], entry[26], entryError[26], entry[27], entryError[27], entry[28], entryError[28], entry[29], entryError[29], entry[30], entryError[30], entry[31], entryError[31], entry[32], entryError[32], entry[33], entryError[33], entry[34], entryError[34], entry[35], entryError[35], entry[36], entryError[36], entry[37], entryError[37], entry[38], entryError[38], entry[39], entryError[39], entry[40], entryError[40], entry[41], entryError[41], entry[42], entryError[42], entry[43], entryError[43], entry[44], entryError[44], entry[45], entryError[45], entry[46], entryError[46], entry[47], entryError[47], entry[48], entryError[48], entry[49], entryError[49], entry[50], entryError[50], entry[51], entryError[51], entry[52], entryError[52], entry[53], entryError[53], entry[54], entryError[54], entry[55], entryError[55], entry[56], entryError[56], entry[57], entryError[57], entry[58], entryError[58], entry[59], entryError[59]) ) {
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

TExperimentInfoSet::~TExperimentInfoSet() { }

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
