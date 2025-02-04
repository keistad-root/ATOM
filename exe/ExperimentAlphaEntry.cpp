#include<csv.h>
#include<array>
#include<iostream>
#include<unordered_map>
#include<cmath>
#include<fstream>

int main() {
	io::CSVReader<66> csv("/home/ychoi/ATOM/Data/experiment.csv");
	csv.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "MINUTE", "CS1", "CS2", "CS3", "CS4", "CS5", "CS6", "CS7", "CS8", "CS9", "CS10", "CS11", "CS12", "CS13", "CS14", "CS15", "CS16", "CS17", "CS18", "CS19", "CS20", "CS21", "CS22", "CS23", "CS24", "CS25", "CS26", "CS27", "CS28", "CS29", "CS30", "CS31", "CS32", "CS33", "CS34", "CS35", "CS36", "CS37", "CS38", "CS39", "CS40", "CS41", "CS42", "CS43", "CS44", "CS45", "CS46", "CS47", "CS48", "CS49", "CS50", "CS51", "CS52", "CS53", "CS54", "CS55", "CS56", "CS57", "CS58", "CS59", "CS60");

	std::string tag;
	int length, phi;
	double collimatorLength, collimatorArea;
	int minute;
	std::array<double, 60> counts;

	std::unordered_map<std::string, std::array<double, 60>> alphaGammaData;
	std::unordered_map<std::string, std::array<double, 60>> alphaGammaError;
	std::unordered_map<std::string, std::array<double, 60>> gammaData;
	std::unordered_map<std::string, std::array<double, 60>> gammaError;

	while ( csv.read_row(tag, length, phi, collimatorLength, collimatorArea, minute, counts[0], counts[1], counts[2], counts[3], counts[4], counts[5], counts[6], counts[7], counts[8], counts[9], counts[10], counts[11], counts[12], counts[13], counts[14], counts[15], counts[16], counts[17], counts[18], counts[19], counts[20], counts[21], counts[22], counts[23], counts[24], counts[25], counts[26], counts[27], counts[28], counts[29], counts[30], counts[31], counts[32], counts[33], counts[34], counts[35], counts[36], counts[37], counts[38], counts[39], counts[40], counts[41], counts[42], counts[43], counts[44], counts[45], counts[46], counts[47], counts[48], counts[49], counts[50], counts[51], counts[52], counts[53], counts[54], counts[55], counts[56], counts[57], counts[58], counts[59]) ) {
		std::array<double, 60> errors;
		for ( int i = 0; i < 60; i++ ) {
			errors[i] = std::sqrt(counts[i] * 10. / minute) * minute / 10.;
		}
		if ( tag.back() == 'O' ) {
			gammaData.insert_or_assign(tag.substr(0, tag.size() - 1), counts);
			gammaError.insert_or_assign(tag.substr(0, tag.size() - 1), errors);
		} else {
			alphaGammaData.insert_or_assign(tag.substr(0, tag.size() - 1), counts);
			alphaGammaError.insert_or_assign(tag.substr(0, tag.size() - 1), errors);
		}
	}

	std::unordered_map<std::string, std::array<double, 60>> alphaData;
	std::unordered_map<std::string, std::array<double, 60>> alphaError;

	for ( auto [tag, counts] : alphaGammaData ) {
		std::cout << tag << std::endl;
		std::array<double, 60> alphaCounts;
		std::array<double, 60> alphaErrors;
		std::array<double, 60> gammaCounts = gammaData.find(tag)->second;
		std::array<double, 60> gammaErrors = gammaError.find(tag)->second;
		std::array<double, 60> alphaGammaCounts = alphaGammaData.find(tag)->second;
		std::array<double, 60> alphaGammaErrors = alphaGammaError.find(tag)->second;
		for ( int i = 0; i < 60; i++ ) {
			alphaCounts[i] = alphaGammaCounts[i] - gammaCounts[i];
			alphaErrors[i] = std::sqrt(std::pow(alphaGammaErrors[i], 2) + std::pow(gammaErrors[i], 2));
		}
		alphaData.insert_or_assign(tag, alphaCounts);
		alphaError.insert_or_assign(tag, alphaErrors);
	}

	std::ofstream file("/home/ychoi/ATOM/Data/experiment_alpha.csv", std::ios::app);
	io::CSVReader<6> csvWriter("/home/ychoi/ATOM/Data/experiment.csv");
	csvWriter.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "MINUTE");
	std::string nTag;
	int nLength, nPhi;
	double nCollimatorLength, nCollimatorArea;
	int nMinute;
	std::vector<std::string> tags;
	while ( csvWriter.read_row(nTag, nLength, nPhi, nCollimatorLength, nCollimatorArea, nMinute) ) {
		if ( std::find(tags.begin(), tags.end(), nTag.substr(0, nTag.size() - 1)) != tags.end() ) {
			continue;
		} else {
			file << nTag.substr(0, nTag.size() - 1) << ", " << nLength << ", " << nPhi << ", " << nCollimatorLength << ", " << nCollimatorArea << ", " << nMinute;
			std::array<double, 60> alphaCounts = alphaData.find(nTag.substr(0, nTag.size() - 1))->second;
			std::array<double, 60> alphaErrors = alphaError.find(nTag.substr(0, nTag.size() - 1))->second;
			for ( int i = 0; i < 60; i++ ) {
				file << ", " << alphaCounts[i] << ", " << alphaErrors[i];
			}
			file << std::endl;
			tags.push_back(nTag.substr(0, nTag.size() - 1));
		}
	}
}