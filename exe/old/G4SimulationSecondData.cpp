#include<string>
#include<csv.h>
#include<iostream>

const std::string INPUT_DATA_FILE = "/home/ychoi/ATOM/Data/g4data.csv";
const std::string OUPTUT_DATA_FILE = "/home/ychoi/ATOM/Data/g4second_data.csv";

int main() {
	io::CSVReader<8> csv(INPUT_DATA_FILE);
	csv.read_header(io::ignore_extra_column, "Tag", "Length", "Phi", "Area", "EEM", "EAM", "EAE", "Double");

	std::string tag;
	int length, phi;
	double area, eem, eam, eae, doubleValue;

	std::vector<std::tuple<std::string, int, int, double, double, double, double, double>> data;

	while ( csv.read_row(tag, length, phi, area, eem, eam, eae, doubleValue) ) {
		data.push_back(std::make_tuple(tag, length, phi, area, eem, eam, eae, doubleValue));
	}

	std::vector<std::tuple<std::string, int, int, double, double, double, double, double>> noShieldData;

	for ( const auto& row : data ) {
		if ( std::get<0>(row).back() == 'X' ) {
			noShieldData.push_back(row);
		}
	}

	std::vector<std::tuple<std::string, int, int, double, double, double, double, double>> alphaData;

	for ( const auto& row : data ) {
		if ( std::get<0>(row).back() == 'O' ) {
			for ( const auto& noShieldRow : noShieldData ) {
				if ( std::get<1>(row) == std::get<1>(noShieldRow) && std::get<2>(row) == std::get<2>(noShieldRow) ) {
					double EEM = std::get<4>(noShieldRow) - std::get<4>(row);
					double EAM = std::get<5>(noShieldRow) - std::get<5>(row);
					double EAE = std::get<6>(noShieldRow) - std::get<6>(row);
					double Double = std::get<7>(noShieldRow) - std::get<7>(row);
					alphaData.push_back(std::make_tuple(std::get<0>(noShieldRow), std::get<1>(row), std::get<2>(row), std::get<3>(row), EEM, EAM, EAE, Double));
				}
			}
		}
	}



}