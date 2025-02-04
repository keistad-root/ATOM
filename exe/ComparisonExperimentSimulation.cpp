#include<csv.h>
#include<array>
#include<vector>
#include<iostream>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TF1.h"


class ExperimentInfo {
public:
	ExperimentInfo(const std::string& tag, int length, int phi, double collimatorLength, double collimatorArea, int minute) : mTag(tag), mLength(length), mPhi(phi), mCollimatorLength(collimatorLength), mCollimatorArea(collimatorArea), mMinute(minute) { };
	~ExperimentInfo() { };

private:
	std::string mTag;
	int mLength;
	int mPhi;
	double mCollimatorLength;
	double mCollimatorArea;
	int mMinute;
	std::array<double, 60> mEntry;
	std::array<double, 60> mError;
public:
	void setEntry(const std::array<double, 60>& entry) { mEntry = entry; }
	void setError(const std::array<double, 60>& error) { mError = error; }
	const std::string& getTag() const { return mTag; }
	int getLength() const { return mLength; }
	int getPhi() const { return mPhi; }
	double getCollimatorLength() const { return mCollimatorLength; }
	double getCollimatorArea() const { return mCollimatorArea; }
	int getMinute() const { return mMinute; }
	const std::array<double, 60>& getEntry() const { return mEntry; }
	const std::array<double, 60>& getError() const { return mError; }
	const std::array<double, 2> getSubEntry(int start, int end) const {
		std::array<double, 2> subEntry = {0, 0};
		for ( int i = start - 1; i < end; i++ ) {
			subEntry[0] += mEntry[i];
			subEntry[1] += mError[i];
		}
		return subEntry;
	}
};

class SimulationInfo {
	std::string mTag;
	int mLength;
	int mPhi;
	double mCollimatorLength;
	double mCollimatorArea;
	int mNEvent;
	std::array<double, 4> mEntry;

};

std::vector<ExperimentInfo> getExperimentSet();

int main() {
	std::vector<ExperimentInfo> expData = getExperimentSet();
	std::cout << expData[0].getTag() << " " << expData[0].getSubEntry(1, 2)[0] << std::endl;
	return 0;
}


std::vector<ExperimentInfo> getExperimentSet() {
	io::CSVReader<126> expCSV("/home/ychoi/ATOM/Data/experiment_alpha.csv");
	expCSV.read_header(io::ignore_extra_column, "TAG", "LENGTH", "PHI", "COLLIMATOR_LENGTH", "COLLIMATOR_AREA", "MINUTE", "CS1", "CS1_ERROR", "CS2", "CS2_ERROR", "CS3", "CS3_ERROR", "CS4", "CS4_ERROR", "CS5", "CS5_ERROR", "CS6", "CS6_ERROR", "CS7", "CS7_ERROR", "CS8", "CS8_ERROR", "CS9", "CS9_ERROR", "CS10", "CS10_ERROR", "CS11", "CS11_ERROR", "CS12", "CS12_ERROR", "CS13", "CS13_ERROR", "CS14", "CS14_ERROR", "CS15", "CS15_ERROR", "CS16", "CS16_ERROR", "CS17", "CS17_ERROR", "CS18", "CS18_ERROR", "CS19", "CS19_ERROR", "CS20", "CS20_ERROR", "CS21", "CS21_ERROR", "CS22", "CS22_ERROR", "CS23", "CS23_ERROR", "CS24", "CS24_ERROR", "CS25", "CS25_ERROR", "CS26", "CS26_ERROR", "CS27", "CS27_ERROR", "CS28", "CS28_ERROR", "CS29", "CS29_ERROR", "CS30", "CS30_ERROR", "CS31", "CS31_ERROR", "CS32", "CS32_ERROR", "CS33", "CS33_ERROR", "CS34", "CS34_ERROR", "CS35", "CS35_ERROR", "CS36", "CS36_ERROR", "CS37", "CS37_ERROR", "CS38", "CS38_ERROR", "CS39", "CS39_ERROR", "CS40", "CS40_ERROR", "CS41", "CS41_ERROR", "CS42", "CS42_ERROR", "CS43", "CS43_ERROR", "CS44", "CS44_ERROR", "CS45", "CS45_ERROR", "CS46", "CS46_ERROR", "CS47", "CS47_ERROR", "CS48", "CS48_ERROR", "CS49", "CS49_ERROR", "CS50", "CS50_ERROR", "CS51", "CS51_ERROR", "CS52", "CS52_ERROR", "CS53", "CS53_ERROR", "CS54", "CS54_ERROR", "CS55", "CS55_ERROR", "CS56", "CS56_ERROR", "CS57", "CS57_ERROR", "CS58", "CS58_ERROR", "CS59", "CS59_ERROR", "CS60", "CS60_ERROR");
	std::string tag;
	int length, phi, minute;
	double collimatorLength, collimatorArea;
	std::array<double, 60> entry;
	std::array<double, 60> entryError;
	std::vector<ExperimentInfo> expData;
	while ( expCSV.read_row(tag, length, phi, collimatorLength, collimatorArea, minute, entry[0], entryError[0], entry[1], entryError[0], entry[2], entryError[2], entry[3], entryError[3], entry[4], entryError[4], entry[5], entryError[5], entry[6], entryError[6], entry[7], entryError[7], entry[8], entryError[8], entry[9], entryError[9], entry[10], entryError[10], entry[11], entryError[11], entry[12], entryError[12], entry[13], entryError[13], entry[14], entryError[14], entry[15], entryError[15], entry[16], entryError[16], entry[17], entryError[17], entry[18], entryError[18], entry[19], entryError[19], entry[20], entryError[20], entry[21], entryError[21], entry[22], entryError[22], entry[23], entryError[23], entry[24], entryError[24], entry[25], entryError[25], entry[26], entryError[26], entry[27], entryError[27], entry[28], entryError[28], entry[29], entryError[29], entry[30], entryError[30], entry[31], entryError[31], entry[32], entryError[32], entry[33], entryError[33], entry[34], entryError[34], entry[35], entryError[35], entry[36], entryError[36], entry[37], entryError[37], entry[38], entryError[38], entry[39], entryError[39], entry[40], entryError[40], entry[41], entryError[41], entry[42], entryError[42], entry[43], entryError[43], entry[44], entryError[44], entry[45], entryError[45], entry[46], entryError[46], entry[47], entryError[47], entry[48], entryError[48], entry[49], entryError[49], entry[50], entryError[50], entry[51], entryError[51], entry[52], entryError[52], entry[53], entryError[53], entry[54], entryError[54], entry[55], entryError[55], entry[56], entryError[56], entry[57], entryError[57], entry[58], entryError[58], entry[59], entryError[59]) ) {
		ExperimentInfo expInfo(tag, length, phi, collimatorLength, collimatorArea, minute);
		expInfo.setEntry(entry);
		expInfo.setError(entryError);
		expData.push_back(expInfo);
	}
	return expData;
}


// std::vector<std::tuple<int, int, std::array<double, 4>>> getSimulationData() {
// 	io::CSVReader<9> simCSV("/home/ychoi/ATOM/Data/simulation_entry.csv");

// 	simCSV.read_header(io::ignore_extra_column, "Collimator", "Length", "Width", "Area", "Area_Error", "Region_A", "Region_B", "Region_C", "Region_D");

// 	std::string collimator;
// 	int length, width;
// 	double area, areaError, regionA, regionB, regionC, regionD;

// 	std::vector<std::tuple<int, int, std::array<double, 4>>> simRefData;
// 	std::vector<std::tuple<int, int, std::array<double, 4>>> simData;

// 	while ( simCSV.read_row(collimator, length, width, area, areaError, regionA, regionB, regionC, regionD) ) {
// 		if ( width == 11 ) {
// 			simRefData.push_back(std::make_tuple(length, width, std::array<double, 4>{regionA, regionB, regionC, regionD}));
// 		} else {
// 			simData.push_back(std::make_tuple(length, width, std::array<double, 4>{regionA, regionB, regionC, regionD}));
// 		}
// 	}
// 	std::vector<std::tuple<int, int, std::array<double, 4>>> simRatioData;

// 	for ( auto& simDataEntry : simData ) {
// 		for ( auto& simRefDataEntry : simRefData ) {
// 			if ( std::get<0>(simDataEntry) == std::get<0>(simRefDataEntry) ) {
// 				std::array<double, 4> ratio = {std::get<2>(simDataEntry)[0] / std::get<2>(simRefDataEntry)[0], std::get<2>(simDataEntry)[1] / std::get<2>(simRefDataEntry)[1], std::get<2>(simDataEntry)[2] / std::get<2>(simRefDataEntry)[2], std::get<2>(simDataEntry)[3] / std::get<2>(simRefDataEntry)[3]};

// 				simRatioData.push_back(std::make_tuple(std::get<0>(simDataEntry), std::get<1>(simDataEntry), ratio));
// 			}
// 		}
// 	}
// 	return simRatioData;
// }

// std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> getExperimentData() {
// 	io::CSVReader<63> expCSV("/home/ychoi/ATOM/Data/clustersize_entry.csv");

// 	expCSV.read_header(io::ignore_extra_column, "length", "width", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "60", "61");

// 	std::tuple<int, int, std::array<double, 61>> expEntry;
// 	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expData;
// 	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expRefData;
// 	// std::vector<std::array<int, 2>> regionDivide = {{1, 4}, {5, 10}, {11, 32}, {40, 61}};
// 	std::vector<std::array<int, 2>> regionDivide = {{4, 32}, {5, 10}, {11, 32}, {5, 32}};
// 	// std::vector<std::array<int, 2>> regionDivide = {{1, 1}, {5, 10}, {11, 32}, {5, 32}};
// 	// std::vector<std::array<int, 2>> regionDivide = {{4, 4}, {5, 10}, {11, 32}, {5, 32}};
// 	std::array<double, 4> regionEntry = {0, 0, 0, 0};
// 	std::array<double, 4> regionEntryError = {0, 0, 0, 0};

// 	while ( expCSV.read_row(std::get<0>(expEntry), std::get<1>(expEntry), std::get<2>(expEntry)[0], std::get<2>(expEntry)[1], std::get<2>(expEntry)[2], std::get<2>(expEntry)[3], std::get<2>(expEntry)[4], std::get<2>(expEntry)[5], std::get<2>(expEntry)[6], std::get<2>(expEntry)[7], std::get<2>(expEntry)[8], std::get<2>(expEntry)[9], std::get<2>(expEntry)[10], std::get<2>(expEntry)[11], std::get<2>(expEntry)[12], std::get<2>(expEntry)[13], std::get<2>(expEntry)[14], std::get<2>(expEntry)[15], std::get<2>(expEntry)[16], std::get<2>(expEntry)[17], std::get<2>(expEntry)[18], std::get<2>(expEntry)[19], std::get<2>(expEntry)[20], std::get<2>(expEntry)[21], std::get<2>(expEntry)[22], std::get<2>(expEntry)[23], std::get<2>(expEntry)[24], std::get<2>(expEntry)[25], std::get<2>(expEntry)[26], std::get<2>(expEntry)[27], std::get<2>(expEntry)[28], std::get<2>(expEntry)[29], std::get<2>(expEntry)[30], std::get<2>(expEntry)[31], std::get<2>(expEntry)[32], std::get<2>(expEntry)[33], std::get<2>(expEntry)[34], std::get<2>(expEntry)[35], std::get<2>(expEntry)[36], std::get<2>(expEntry)[37], std::get<2>(expEntry)[38], std::get<2>(expEntry)[39], std::get<2>(expEntry)[40], std::get<2>(expEntry)[41], std::get<2>(expEntry)[42], std::get<2>(expEntry)[43], std::get<2>(expEntry)[44], std::get<2>(expEntry)[45], std::get<2>(expEntry)[46], std::get<2>(expEntry)[47], std::get<2>(expEntry)[48], std::get<2>(expEntry)[49], std::get<2>(expEntry)[50], std::get<2>(expEntry)[51], std::get<2>(expEntry)[52], std::get<2>(expEntry)[53], std::get<2>(expEntry)[54], std::get<2>(expEntry)[55], std::get<2>(expEntry)[56], std::get<2>(expEntry)[57], std::get<2>(expEntry)[58], std::get<2>(expEntry)[59], std::get<2>(expEntry)[60]) ) {
// 		for ( int i = 0; i < std::get<2>(expEntry).size(); i++ ) {
// 			if ( i >= regionDivide[0][0] - 1 && i <= regionDivide[0][1] - 1 ) {
// 				regionEntry[0] += std::get<2>(expEntry)[i];
// 			}
// 			if ( i >= regionDivide[1][0] - 1 && i <= regionDivide[1][1] - 1 ) {
// 				regionEntry[1] += std::get<2>(expEntry)[i];
// 			}
// 			if ( i >= regionDivide[2][0] - 1 && i <= regionDivide[2][1] - 1 ) {
// 				regionEntry[2] += std::get<2>(expEntry)[i];
// 			}
// 			if ( i >= regionDivide[3][0] - 1 && i <= regionDivide[3][1] - 1 ) {
// 				regionEntry[3] += std::get<2>(expEntry)[i];
// 			}
// 		}
// 		if ( std::get<1>(expEntry) == 11 ) {
// 			expRefData.push_back(std::make_tuple(std::get<0>(expEntry), std::get<1>(expEntry), regionEntry, regionEntryError));
// 		} else {
// 			expData.push_back(std::make_tuple(std::get<0>(expEntry), std::get<1>(expEntry), regionEntry, regionEntryError));
// 		}
// 		regionEntry = {0, 0, 0, 0};
// 	}

// 	io::CSVReader<4> scaleCSV("/home/ychoi/ATOM/Data/time_scale.csv");
// 	scaleCSV.read_header(io::ignore_extra_column, "length", "width", "time", "scale");
// 	int length, width, time;
// 	double scale;
// 	while ( scaleCSV.read_row(length, width, time, scale) ) {
// 		for ( auto& expDataEntry : expData ) {
// 			if ( std::get<0>(expDataEntry) == length && std::get<1>(expDataEntry) == width ) {
// 				regionEntryError[0] = TMath::Sqrt(TMath::Abs(std::get<2>(expDataEntry)[0] / scale)) * scale;
// 				regionEntryError[1] = TMath::Sqrt(TMath::Abs(std::get<2>(expDataEntry)[1] / scale)) * scale;
// 				regionEntryError[2] = TMath::Sqrt(TMath::Abs(std::get<2>(expDataEntry)[2] / scale)) * scale;
// 				regionEntryError[3] = TMath::Sqrt(TMath::Abs(std::get<2>(expDataEntry)[3] / scale)) * scale;
// 				std::get<3>(expDataEntry) = regionEntryError;
// 			}
// 		}
// 	}
// 	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expRatioData;
// 	for ( auto& expDataEntry : expData ) {
// 		for ( auto& expRefDataEntry : expRefData ) {
// 			if ( std::get<0>(expDataEntry) == std::get<0>(expRefDataEntry) ) {
// 				std::array<double, 4> ratio = {std::get<2>(expDataEntry)[0] / std::get<2>(expRefDataEntry)[0], std::get<2>(expDataEntry)[1] / std::get<2>(expRefDataEntry)[1], std::get<2>(expDataEntry)[2] / std::get<2>(expRefDataEntry)[2], std::get<2>(expDataEntry)[3] / std::get<2>(expRefDataEntry)[3]};

// 				std::array<double, 4> ratioError = {TMath::Sqrt(TMath::Power(std::get<3>(expDataEntry)[0] / std::get<2>(expRefDataEntry)[0], 2) + TMath::Power(std::get<2>(expDataEntry)[0] * std::get<3>(expRefDataEntry)[0] / TMath::Power(std::get<2>(expRefDataEntry)[0], 2), 2)), TMath::Sqrt(TMath::Power(std::get<3>(expDataEntry)[1] / std::get<2>(expRefDataEntry)[1], 2) + TMath::Power(std::get<2>(expDataEntry)[1] * std::get<3>(expRefDataEntry)[1] / TMath::Power(std::get<2>(expRefDataEntry)[1], 2), 2)), TMath::Sqrt(TMath::Power(std::get<3>(expDataEntry)[2] / std::get<2>(expRefDataEntry)[2], 2) + TMath::Power(std::get<2>(expDataEntry)[2] * std::get<3>(expRefDataEntry)[2] / TMath::Power(std::get<2>(expRefDataEntry)[2], 2), 2)), TMath::Sqrt(TMath::Power(std::get<3>(expDataEntry)[3] / std::get<2>(expRefDataEntry)[3], 2) + TMath::Power(std::get<2>(expDataEntry)[3] * std::get<3>(expRefDataEntry)[3] / TMath::Power(std::get<2>(expRefDataEntry)[3], 2), 2))};

// 				expRatioData.push_back(std::make_tuple(std::get<0>(expDataEntry), std::get<1>(expDataEntry), ratio, ratioError));
// 			}
// 		}
// 	}
// 	return expRatioData;
// }

// void drawForLength(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == drawWidth ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}
// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == drawWidth ) {
// 			simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
// 			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
// 			simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	expGraphPhi2[3]->SetLineWidth(2);
// 	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	expGraphPhi2[3]->SetMarkerStyle(24);
// 	expGraphPhi2[3]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	simGraphPhi[0]->SetLineColor(kBlue);
// 	simGraphPhi[0]->SetLineWidth(2);
// 	simGraphPhi[0]->SetLineStyle(9);
// 	simGraphPhi[0]->SetMarkerColor(kBlue);
// 	simGraphPhi[0]->SetMarkerSize(2);
// 	simGraphPhi[0]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[0]);
// 	simGraphPhi[1]->SetLineColor(kRed);
// 	simGraphPhi[1]->SetLineWidth(2);
// 	simGraphPhi[1]->SetLineStyle(9);
// 	simGraphPhi[1]->SetMarkerColor(kRed);
// 	simGraphPhi[1]->SetMarkerSize(2);
// 	simGraphPhi[1]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[1]);
// 	simGraphPhi[2]->SetLineColor(kMagenta);
// 	simGraphPhi[2]->SetLineWidth(2);
// 	simGraphPhi[2]->SetLineStyle(9);
// 	simGraphPhi[2]->SetMarkerColor(kMagenta);
// 	simGraphPhi[2]->SetMarkerSize(2);
// 	simGraphPhi[2]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[2]);
// 	simGraphPhi[3]->SetLineColor(kGreen + 3);
// 	simGraphPhi[3]->SetLineWidth(2);
// 	simGraphPhi[3]->SetLineStyle(9);
// 	simGraphPhi[3]->SetMarkerColor(kGreen + 3);
// 	simGraphPhi[3]->SetMarkerSize(2);
// 	simGraphPhi[3]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[3]);
// 	mgSimPhi->Draw("P");

// 	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Region A", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
// 	legendPhi->AddEntry(expGraphPhi2[3], "Region D", "p");
// 	legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
// 	legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
// 	legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
// 	legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference.png"));
// 	delete canvasPhi;
// }

// void drawOnlyBC(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == drawWidth ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}
// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == drawWidth ) {
// 			// simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
// 			// simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
// 			// simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	// expGraphPhi2[0]->SetLineColor(kRed);
// 	// expGraphPhi2[0]->SetLineWidth(2);
// 	// expGraphPhi2[0]->SetMarkerColor(kRed);
// 	// expGraphPhi2[0]->SetMarkerStyle(24);
// 	// expGraphPhi2[0]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	expGraphPhi2[3]->SetLineWidth(2);
// 	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	expGraphPhi2[3]->SetMarkerStyle(24);
// 	expGraphPhi2[3]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
// 	// mgPhi->SetMinimum(0);
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	// simGraphPhi[0]->SetLineColor(kBlue);
// 	// simGraphPhi[0]->SetLineWidth(2);
// 	// simGraphPhi[0]->SetLineStyle(9);
// 	// simGraphPhi[0]->SetMarkerColor(kBlue);
// 	// simGraphPhi[0]->SetMarkerSize(2);
// 	// simGraphPhi[0]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi[0]);
// 	// simGraphPhi[1]->SetLineColor(kRed);
// 	// simGraphPhi[1]->SetLineWidth(2);
// 	// simGraphPhi[1]->SetLineStyle(9);
// 	// simGraphPhi[1]->SetMarkerColor(kRed);
// 	// simGraphPhi[1]->SetMarkerSize(2);
// 	// simGraphPhi[1]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi[1]);
// 	simGraphPhi[2]->SetLineColor(kMagenta);
// 	simGraphPhi[2]->SetLineWidth(2);
// 	simGraphPhi[2]->SetLineStyle(9);
// 	simGraphPhi[2]->SetMarkerColor(kMagenta);
// 	simGraphPhi[2]->SetMarkerSize(2);
// 	simGraphPhi[2]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[2]);
// 	// simGraphPhi[3]->SetLineColor(kGreen + 3);
// 	// simGraphPhi[3]->SetLineWidth(2);
// 	// simGraphPhi[3]->SetLineStyle(9);
// 	// simGraphPhi[3]->SetMarkerColor(kGreen + 3);
// 	// simGraphPhi[3]->SetMarkerSize(2);
// 	// simGraphPhi[3]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[3]);
// 	mgSimPhi->Draw("P");

// 	TLegend* legendPhi = new TLegend(0.3, 0.7, 0.9, 0.9);
// 	// legendPhi->AddEntry(expGraphPhi2[0], "Region B + C + cluster size 4", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
// 	legendPhi->AddEntry(expGraphPhi2[3], "Region B + C", "p");
// 	// legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
// 	legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.13);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_BC.png"));
// 	delete canvasPhi;
// }


// void drawOnlyA(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == drawWidth ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}
// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == drawWidth ) {
// 			// simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[0]);
// 			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 			// simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[2]);
// 			// simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	// expGraphPhi2[1]->SetLineColor(kBlue);
// 	// expGraphPhi2[1]->SetLineWidth(2);
// 	// expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	// expGraphPhi2[1]->SetMarkerStyle(24);
// 	// expGraphPhi2[1]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[1]);
// 	// expGraphPhi2[2]->SetLineColor(kMagenta);
// 	// expGraphPhi2[2]->SetLineWidth(2);
// 	// expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	// expGraphPhi2[2]->SetMarkerStyle(24);
// 	// expGraphPhi2[2]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[2]);
// 	// expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetLineWidth(2);
// 	// expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetMarkerStyle(24);
// 	// expGraphPhi2[3]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
// 	mgPhi->SetMinimum(0);
// 	mgPhi->SetMaximum(.8);
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	// simGraphPhi[0]->SetLineColor(kBlue);
// 	// simGraphPhi[0]->SetLineWidth(2);
// 	// simGraphPhi[0]->SetLineStyle(9);
// 	// simGraphPhi[0]->SetMarkerColor(kBlue);
// 	// simGraphPhi[0]->SetMarkerSize(2);
// 	// simGraphPhi[0]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi[0]);
// 	simGraphPhi[1]->SetLineColor(kRed);
// 	simGraphPhi[1]->SetLineWidth(2);
// 	simGraphPhi[1]->SetLineStyle(9);
// 	simGraphPhi[1]->SetMarkerColor(kRed);
// 	simGraphPhi[1]->SetMarkerSize(2);
// 	simGraphPhi[1]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[1]);
// 	// simGraphPhi[2]->SetLineColor(kMagenta);
// 	// simGraphPhi[2]->SetLineWidth(2);
// 	// simGraphPhi[2]->SetLineStyle(9);
// 	// simGraphPhi[2]->SetMarkerColor(kMagenta);
// 	// simGraphPhi[2]->SetMarkerSize(2);
// 	// simGraphPhi[2]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi[2]);
// 	// simGraphPhi[3]->SetLineColor(kGreen + 3);
// 	// simGraphPhi[3]->SetLineWidth(2);
// 	// simGraphPhi[3]->SetLineStyle(9);
// 	// simGraphPhi[3]->SetMarkerColor(kGreen + 3);
// 	// simGraphPhi[3]->SetMarkerSize(2);
// 	// simGraphPhi[3]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[3]);
// 	mgSimPhi->Draw("P");

// 	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Cluster size 1", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[3], "Region B + C", "p");
// 	legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_1.png"));
// 	delete canvasPhi;
// }


// void drawOnly4(int drawWidth, std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == drawWidth ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[1]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[1]);
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[2]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[2]);
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	// expGraphPhi2[1]->SetLineColor(kBlue);
// 	// expGraphPhi2[1]->SetLineWidth(2);
// 	// expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	// expGraphPhi2[1]->SetMarkerStyle(24);
// 	// expGraphPhi2[1]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[1]);
// 	// expGraphPhi2[2]->SetLineColor(kMagenta);
// 	// expGraphPhi2[2]->SetLineWidth(2);
// 	// expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	// expGraphPhi2[2]->SetMarkerStyle(24);
// 	// expGraphPhi2[2]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[2]);
// 	// expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetLineWidth(2);
// 	// expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetMarkerStyle(24);
// 	// expGraphPhi2[3]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Comparison for " + std::to_string(drawWidth) + "#phi collimators; Length[mm]; Ratio to Reference"));
// 	mgPhi->Draw("AP");

// 	TLegend* legendPhi = new TLegend(0.3, 0.6, 0.8, 0.9);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Cluster size 4", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[1], "Region B", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[2], "Region C", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[3], "Region B + C", "p");
// 	// legendPhi->AddEntry(simGraphPhi[1], "The # of electron in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[0], "The # of single alpha in metal (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[2], "The # of single alpha in epitaxial (Simulation)", "p");
// 	// legendPhi->AddEntry(simGraphPhi[3], "The # of double cluster (Simulation)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("phi" + std::to_string(drawWidth) + "_ratio_to_reference_4.png"));
// 	delete canvasPhi;
// }

// void drawOnly1All(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>> simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraph* simGraphPhi[4] = {new TGraph(), new TGraph(), new TGraph(), new TGraph()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == 2 ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 3 ) {
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 4 ) {
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 7 ) {
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		}
// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == 2 ) {
// 			simGraphPhi[0]->SetPoint(simGraphPhi[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 		} else if ( std::get<1>(simDataEntry) == 3 ) {
// 			simGraphPhi[1]->SetPoint(simGraphPhi[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 		} else if ( std::get<1>(simDataEntry) == 4 ) {
// 			simGraphPhi[2]->SetPoint(simGraphPhi[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 		} else if ( std::get<1>(simDataEntry) == 7 ) {
// 			simGraphPhi[3]->SetPoint(simGraphPhi[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[1]);
// 		}
// 	}
// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	expGraphPhi2[3]->SetLineWidth(2);
// 	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	expGraphPhi2[3]->SetMarkerStyle(24);
// 	expGraphPhi2[3]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Cluster Size 1 vs. Electrons with full energy deposit in metal layer; Length [mm]; Ratio to Reference"));
// 	mgPhi->SetMaximum(0.8);
// 	mgPhi->SetMinimum(0);
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	simGraphPhi[0]->SetLineColor(kRed);
// 	simGraphPhi[0]->SetLineWidth(2);
// 	simGraphPhi[0]->SetLineStyle(9);
// 	simGraphPhi[0]->SetMarkerColor(kRed);
// 	simGraphPhi[0]->SetMarkerSize(2);
// 	simGraphPhi[0]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[0]);
// 	simGraphPhi[1]->SetLineColor(kBlue);
// 	simGraphPhi[1]->SetLineWidth(2);
// 	simGraphPhi[1]->SetLineStyle(9);
// 	simGraphPhi[1]->SetMarkerColor(kBlue);
// 	simGraphPhi[1]->SetMarkerSize(2);
// 	simGraphPhi[1]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[1]);
// 	simGraphPhi[2]->SetLineColor(kMagenta);
// 	simGraphPhi[2]->SetLineWidth(2);
// 	simGraphPhi[2]->SetLineStyle(9);
// 	simGraphPhi[2]->SetMarkerColor(kMagenta);
// 	simGraphPhi[2]->SetMarkerSize(2);
// 	simGraphPhi[2]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[2]);
// 	simGraphPhi[3]->SetLineColor(kGreen + 3);
// 	simGraphPhi[3]->SetLineWidth(2);
// 	simGraphPhi[3]->SetLineStyle(9);
// 	simGraphPhi[3]->SetMarkerColor(kGreen + 3);
// 	simGraphPhi[3]->SetMarkerSize(2);
// 	simGraphPhi[3]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi[3]);
// 	mgSimPhi->Draw("P");

// 	TF1* fitFunc[4] = {new TF1("fitFunc1", "[0] / (x - [1]) + [2]", 0, 20), new TF1("fitFunc2", "[0] / (x - [1]) + [2]", 0, 20), new TF1("fitFunc3", "[0] / (x - [1]) + [2]", 0, 20), new TF1("fitFunc4", "[0] / (x - [1]) + [2]", 0, 20)};
// 	simGraphPhi[0]->Fit(fitFunc[0], "R");
// 	fitFunc[0]->SetLineColor(kRed);
// 	simGraphPhi[1]->Fit(fitFunc[1], "R");
// 	fitFunc[1]->SetLineColor(kBlue);
// 	simGraphPhi[2]->Fit(fitFunc[2], "R");
// 	fitFunc[2]->SetLineColor(kMagenta);
// 	simGraphPhi[3]->Fit(fitFunc[3], "R");
// 	fitFunc[3]->SetLineColor(kGreen + 3);
// 	fitFunc[0]->Draw("SAME");
// 	fitFunc[1]->Draw("SAME");
// 	fitFunc[2]->Draw("SAME");
// 	fitFunc[3]->Draw("SAME");

// 	TLegend* legendPhi = new TLegend(0.5, 0.7, 0.9, 0.9);
// 	legendPhi->SetNColumns(2);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Data (#phi2)", "p");
// 	legendPhi->AddEntry(simGraphPhi[0], "GEANT4 (#phi2)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Data (#phi3)", "p");
// 	legendPhi->AddEntry(simGraphPhi[1], "GEANT4 (#phi3)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Data (#phi4)", "p");
// 	legendPhi->AddEntry(simGraphPhi[2], "GEANT4 (#phi4)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[3], "Data (#phi7)", "p");
// 	legendPhi->AddEntry(simGraphPhi[3], "GEANT4 (#phi7)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("ratio_to_reference_cluster_size_1.png"));
// }

// void drawOnly4All(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == 2 ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 3 ) {
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 4 ) {
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		} else if ( std::get<1>(expDataEntry) == 7 ) {
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[0]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[0]);
// 		}

// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	expGraphPhi2[3]->SetLineWidth(2);
// 	expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	expGraphPhi2[3]->SetMarkerStyle(24);
// 	expGraphPhi2[3]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Cluster Size 4; Length [mm]; Ratio to Reference"));
// 	mgPhi->Draw("AP");

// 	TLegend* legendPhi = new TLegend(0.5, 0.7, 0.9, 0.9);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Data (#phi2)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Data (#phi3)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Data (#phi4)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[3], "Data (#phi7)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	canvasPhi->SaveAs(static_cast<TString>("ratio_to_reference_cluster_size_4.png"));
// 	delete canvasPhi;
// }

// void drawDoubleCluster(std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>>& expData, std::vector<std::tuple<int, int, std::array<double, 4>>>& simData) {
// 	TGraphErrors* expGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	TGraphErrors* simGraphPhi2[4] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
// 	for ( auto& expDataEntry : expData ) {
// 		if ( std::get<1>(expDataEntry) == 2 ) {
// 			expGraphPhi2[0]->SetPoint(expGraphPhi2[0]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[0]->SetPointError(expGraphPhi2[0]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		} else if ( std::get<1>(expDataEntry) == 3 ) {
// 			expGraphPhi2[1]->SetPoint(expGraphPhi2[1]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[1]->SetPointError(expGraphPhi2[1]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		} else if ( std::get<1>(expDataEntry) == 4 ) {
// 			expGraphPhi2[2]->SetPoint(expGraphPhi2[2]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[2]->SetPointError(expGraphPhi2[2]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		} else if ( std::get<1>(expDataEntry) == 7 ) {
// 			expGraphPhi2[3]->SetPoint(expGraphPhi2[3]->GetN(), std::get<0>(expDataEntry), std::get<2>(expDataEntry)[3]);
// 			expGraphPhi2[3]->SetPointError(expGraphPhi2[3]->GetN() - 1, 0, std::get<3>(expDataEntry)[3]);
// 		}

// 	}
// 	for ( auto& simDataEntry : simData ) {
// 		if ( std::get<1>(simDataEntry) == 2 ) {
// 			simGraphPhi2[0]->SetPoint(simGraphPhi2[0]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		} else if ( std::get<1>(simDataEntry) == 3 ) {
// 			simGraphPhi2[1]->SetPoint(simGraphPhi2[1]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		} else if ( std::get<1>(simDataEntry) == 4 ) {
// 			simGraphPhi2[2]->SetPoint(simGraphPhi2[2]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		} else if ( std::get<1>(simDataEntry) == 7 ) {
// 			simGraphPhi2[3]->SetPoint(simGraphPhi2[3]->GetN(), std::get<0>(simDataEntry), std::get<2>(simDataEntry)[3]);
// 		}
// 	}

// 	TCanvas* canvasPhi = new TCanvas("canvasphi", "", 1000, 1000);
// 	TMultiGraph* mgPhi = new TMultiGraph();
// 	expGraphPhi2[0]->SetLineColor(kRed);
// 	expGraphPhi2[0]->SetLineWidth(2);
// 	expGraphPhi2[0]->SetMarkerColor(kRed);
// 	expGraphPhi2[0]->SetMarkerStyle(24);
// 	expGraphPhi2[0]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[0]);
// 	expGraphPhi2[1]->SetLineColor(kBlue);
// 	expGraphPhi2[1]->SetLineWidth(2);
// 	expGraphPhi2[1]->SetMarkerColor(kBlue);
// 	expGraphPhi2[1]->SetMarkerStyle(24);
// 	expGraphPhi2[1]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[1]);
// 	expGraphPhi2[2]->SetLineColor(kMagenta);
// 	expGraphPhi2[2]->SetLineWidth(2);
// 	expGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	expGraphPhi2[2]->SetMarkerStyle(24);
// 	expGraphPhi2[2]->SetMarkerSize(2);
// 	mgPhi->Add(expGraphPhi2[2]);
// 	// expGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetLineWidth(2);
// 	// expGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	// expGraphPhi2[3]->SetMarkerStyle(24);
// 	// expGraphPhi2[3]->SetMarkerSize(2);
// 	// mgPhi->Add(expGraphPhi2[3]);
// 	mgPhi->SetTitle(static_cast<TString>("Region D (data) vs. Double Cluster; Length [mm]; Ratio to Reference"));
// 	mgPhi->SetMaximum(0.4);
// 	mgPhi->Draw("AP");

// 	TMultiGraph* mgSimPhi = new TMultiGraph();
// 	simGraphPhi2[0]->SetLineColor(kRed);
// 	simGraphPhi2[0]->SetLineWidth(2);
// 	simGraphPhi2[0]->SetLineStyle(9);
// 	simGraphPhi2[0]->SetMarkerColor(kRed);
// 	simGraphPhi2[0]->SetMarkerSize(2);
// 	simGraphPhi2[0]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi2[0]);
// 	simGraphPhi2[1]->SetLineColor(kBlue);
// 	simGraphPhi2[1]->SetLineWidth(2);
// 	simGraphPhi2[1]->SetLineStyle(9);
// 	simGraphPhi2[1]->SetMarkerColor(kBlue);
// 	simGraphPhi2[1]->SetMarkerSize(2);
// 	simGraphPhi2[1]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi2[1]);
// 	simGraphPhi2[2]->SetLineColor(kMagenta);
// 	simGraphPhi2[2]->SetLineWidth(2);
// 	simGraphPhi2[2]->SetLineStyle(9);
// 	simGraphPhi2[2]->SetMarkerColor(kMagenta);
// 	simGraphPhi2[2]->SetMarkerSize(2);
// 	simGraphPhi2[2]->SetMarkerStyle(21);
// 	mgSimPhi->Add(simGraphPhi2[2]);
// 	// simGraphPhi2[3]->SetLineColor(kGreen + 3);
// 	// simGraphPhi2[3]->SetLineWidth(2);
// 	// simGraphPhi2[3]->SetLineStyle(9);
// 	// simGraphPhi2[3]->SetMarkerColor(kGreen + 3);
// 	// simGraphPhi2[3]->SetMarkerSize(2);
// 	// simGraphPhi2[3]->SetMarkerStyle(21);
// 	// mgSimPhi->Add(simGraphPhi2[3]);
// 	mgSimPhi->Draw("P");

// 	TLegend* legendPhi = new TLegend(0.5, 0.7, 0.9, 0.9);
// 	legendPhi->SetNColumns(2);
// 	legendPhi->AddEntry(expGraphPhi2[0], "Data (#phi2)", "p");
// 	legendPhi->AddEntry(simGraphPhi2[0], "GEANT4 (#phi2)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[1], "Data (#phi3)", "p");
// 	legendPhi->AddEntry(simGraphPhi2[1], "GEANT4 (#phi3)", "p");
// 	legendPhi->AddEntry(expGraphPhi2[2], "Data (#phi4)", "p");
// 	// legendPhi->AddEntry(expGraphPhi2[3], "Data (#phi7)", "p");
// 	legendPhi->AddEntry(simGraphPhi2[2], "GEANT4 (#phi4)", "p");
// 	// legendPhi->AddEntry(simGraphPhi2[3], "Geant4 (#phi7)", "p");
// 	legendPhi->Draw("SAME");

// 	canvasPhi->SetLeftMargin(0.12);
// 	canvasPhi->SetGrid();
// 	// canvasPhi->SetLogy();
// 	canvasPhi->SaveAs(static_cast<TString>("double_cluster_ratio_to_reference.png"));
// 	delete canvasPhi;
// }

// int main() {
// 	std::vector<std::tuple<int, int, std::array<double, 4>, std::array<double, 4>>> expData = getExperimentData();
// 	std::vector<std::tuple<int, int, std::array<double, 4>>> simData = getSimulationData();

// 	// drawForLength(2, expData, simData);
// 	// drawForLength(3, expData, simData);
// 	// drawForLength(4, expData, simData);
// 	// drawForLength(7, expData, simData);

// 	drawOnlyBC(2, expData, simData);
// 	drawOnlyBC(3, expData, simData);
// 	drawOnlyBC(4, expData, simData);
// 	drawOnlyBC(7, expData, simData);

// 	// drawOnlyA(2, expData, simData);
// 	// drawOnlyA(3, expData, simData);
// 	// drawOnlyA(4, expData, simData);
// 	// drawOnlyA(7, expData, simData);

// 	// drawOnly4(2, expData, simData);
// 	// drawOnly4(3, expData, simData);
// 	// drawOnly4(4, expData, simData);
// 	// drawOnly4(7, expData, simData);
// 	// drawOnly4All(expData);
// 	// drawDoubleCluster(expData, simData);
// 	// drawOnly1All(expData, simData);


// 	return 0;
// }