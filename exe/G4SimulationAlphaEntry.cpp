#include<csv.h>
#include<unordered_map>
#include<array>
#include<cmath>

int main() {
	io::CSVReader<9> CSV("/home/ychoi/ATOM/Data/g4data.csv");
	CSV.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "EVENT_NUM", "EEM", "EAM", "EAE", "Double");
	std::string tag;
	int length, phi, eventNum;
	double collimatorLength, collimatorArea, eem, eam, eae, doubleCluster;

	std::unordered_map<std::string, std::array<double, 4>> noAlData;
	std::unordered_map<std::string, std::array<double, 4>> noAlError;
	std::unordered_map<std::string, std::array<double, 4>> alData;
	std::unordered_map<std::string, std::array<double, 4>> alError;

	while ( CSV.read_row(tag, length, phi, collimatorLength, collimatorArea, eventNum, eem, eam, eae, doubleCluster) ) {
		std::array<double, 60> errors;
		errors[i] = std::sqrt(counts[i] * 2580000. / eventNum) * eventNum / 2580000.;
	}
}