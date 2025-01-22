#include<string>
#include<csv.h>

const std::string INPUT_DATA_FILE = "/home/ychoi/ATOM/Data/g4data.csv";
const std::string OUPTUT_DATA_FILE = "/home/ychoi/ATOM/Data/g4second_data.csv";

int main() {
	io::CSVReader<8> csv(INPUT_DATA_FILE);
	csv.read_header(io::ignore_extra_column, "Tag", "Length", "Phi", "Area", "EEM", "EAM", "EAE", "Double");

}