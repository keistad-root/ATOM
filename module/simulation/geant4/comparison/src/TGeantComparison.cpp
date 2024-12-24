#include "TGeantComparison.h"

TGeantComparison::TGeantComparison(const CppConfigFile& config) : TPlotter(&config), mConfig(config) {
	mOutputPath = mConfig.getConfig("File").find("output_directory");

	std::vector<std::string> fileNameList;
	std::stringstream fileNameStr(mConfig.getConfig("File").find("file_list"));

	std::string fileName;
	while ( fileNameStr >> fileName ) {
		fileNameList.push_back(fileName);
	}

	std::vector<std::string> referenceList;
	std::stringstream referenceStr(mConfig.getConfig("File").find("reference_list"));

	while ( referenceStr >> fileName ) {
		referenceList.push_back(fileName);
	}

	io::CSVReader<5> fileListCSV("/home/ychoi/ATOM/config/g4simulation/g4comparison_list.csv");
	fileListCSV.read_header(io::ignore_extra_column, "tag", "length", "width", "area", "file");

	int length, width;
	double area;
	std::string tag, file;

	while ( fileListCSV.read_row(tag, length, width, area, file) ) {
		if ( std::find(fileNameList.begin(), fileNameList.end(), tag) != fileNameList.end() ) {
			FileInfo fileInfo;
			fileInfo.length = length;
			fileInfo.width = width;
			fileInfo.area = area;
			fileInfo.file = file;

			mFileInfo.push_back(fileInfo);
		} else if ( std::find(referenceList.begin(), referenceList.end(), tag) != referenceList.end() ) {
			FileInfo fileInfo;
			fileInfo.length = length;
			fileInfo.width = width;
			fileInfo.area = area;
			fileInfo.file = file;

			mReference.push_back(fileInfo);
		}
	}
	for ( FileInfo& fileInfo : mFileInfo ) {
		getIncidentInfo(fileInfo);
	}
	for ( FileInfo& fileInfo : mReference ) {
		getIncidentInfo(fileInfo);
	}

	TCanvas* canvas = new TCanvas("distanceCanvas", "", 1000, 1000);
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	mDistanceHist[0]->SetLineColor(kRed);
	mDistanceHist[0]->SetLineWidth(2);
	mDistanceHist[0]->SetTitle("Distance between Alpha Particles; Distance [mm]; Counts");
	mDistanceHist[0]->SetMaximum(10000);
	mDistanceHist[0]->Draw();
	mDistanceHist[1]->SetLineColor(kBlue);
	mDistanceHist[1]->SetLineWidth(2);
	mDistanceHist[1]->Draw("same");
	mDistanceHist[2]->SetLineColor(kMagenta);
	mDistanceHist[2]->SetLineWidth(2);
	mDistanceHist[2]->Draw("same");

	legend->AddEntry(mDistanceHist[0], "4#phi", "l");
	legend->AddEntry(mDistanceHist[1], "7#phi", "l");
	legend->AddEntry(mDistanceHist[2], "no collimator", "l");
	legend->Draw("same");
	canvas->SaveAs("distance.png");

	io::CSVReader<9> expCSV("/home/ychoi/ATOM/Data/entry.csv");
	expCSV.read_header(io::ignore_extra_column, "Collimator", "Length", "Width", "Area", "Area_Error", "Region_A", "Region_B", "Region_C", "Region_D");

	std::string collimator;
	int expLength, expWidth;
	double expArea, areaError, regionA, regionB, regionC, regionD;

	while ( expCSV.read_row(collimator, expLength, expWidth, expArea, areaError, regionA, regionB, regionC, regionD) ) {
		ExpInfo expInfo;
		expInfo.length = expLength;
		expInfo.width = expWidth;
		expInfo.area = expArea;
		expInfo.areaError = areaError;
		expInfo.regionA = regionA;
		expInfo.regionB = regionB;
		expInfo.regionC = regionC;
		expInfo.regionD = regionD;

		mExpInfo.push_back(expInfo);
	}
}

TGeantComparison::~TGeantComparison() { }

void TGeantComparison::getIncidentInfo(FileInfo& fileInfo) {
	TFile* file = new TFile(static_cast<TString>(fileInfo.file), "READ");
	TTree* tree = static_cast<TTree*>(file->Get("incidentTree"));

	Int_t eventID;
	Int_t particleID;
	Double_t x;
	Double_t y;
	Double_t depositEnergyEpitaxial;

	tree->SetBranchAddress("eventID", &eventID);
	tree->SetBranchAddress("particleID", &particleID);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("y", &y);
	tree->SetBranchAddress("depositEnergyEpitaxial", &depositEnergyEpitaxial);


	Int_t preTimeStamp = -1;
	std::vector<std::pair<Double_t, Double_t>> position;

	if ( fileInfo.length == 1 && fileInfo.width == 4 ) {
		mDistanceHist[0] = new TH1D("distance4", "", 1000, 0, 10);
	} else if ( fileInfo.length == 1 && fileInfo.width == 7 ) {
		mDistanceHist[1] = new TH1D("distance7", "", 1000, 0, 10);
	} else if ( fileInfo.length == 1 && fileInfo.width == 11 ) {
		mDistanceHist[2] = new TH1D("distance10", "", 1000, 0, 10);
	}

	for ( Int_t i = 0; i < tree->GetEntries(); i++ ) {
		tree->GetEntry(i);

		if ( particleID == 1 && depositEnergyEpitaxial > -.5 ) {
			fileInfo.nAlpha++;
			position.push_back({x, y});
		} else if ( particleID == 2 ) {
			fileInfo.nElectron++;
		}
		if ( preTimeStamp != eventID / eventSize ) {
			preTimeStamp = eventID / eventSize;
			fileInfo.nDouble += getDoubleClusterNum(position);
			if ( fileInfo.length == 1 && fileInfo.width == 4 ) {
				getDistance(mDistanceHist[0], position);
			} else if ( fileInfo.length == 1 && fileInfo.width == 7 ) {
				getDistance(mDistanceHist[1], position);
			} else if ( fileInfo.length == 1 && fileInfo.width == 11 ) {
				getDistance(mDistanceHist[2], position);
			}
			position.clear();
		}
	}

}

Int_t TGeantComparison::getDoubleClusterNum(std::vector<std::pair<Double_t, Double_t>> position) {
	Int_t num = 0;
	for ( Int_t i = 0; i < position.size(); i++ ) {
		for ( Int_t j = i + 1; j < position.size(); j++ ) {
			Double_t distance = TMath::Sqrt(TMath::Power(position[i].first - position[j].first, 2) + TMath::Power(position[i].second - position[j].second, 2));
			if ( distance > 0.048 && distance < 0.154 ) {
				num++;
			}
		}
	}
	return num;
}

void TGeantComparison::getDistance(TH1* hist, std::vector<std::pair<Double_t, Double_t>> position) {
	for ( Int_t i = 0; i < position.size(); i++ ) {
		for ( Int_t j = i + 1; j < position.size(); j++ ) {
			Double_t distance = TMath::Sqrt(TMath::Power(position[i].first - position[j].first, 2) + TMath::Power(position[i].second - position[j].second, 2));
			hist->Fill(distance);
		}
	}
}

void TGeantComparison::drawNAlphaGraph() {
	TCanvas* canvas = new TCanvas("nAlphaCanvas", "", 1000, 1000);
	TGraph* graph[4];
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	graph[0] = new TGraph();
	graph[1] = new TGraph();
	graph[2] = new TGraph();
	graph[3] = new TGraph();

	for ( const FileInfo& fileInfo : mFileInfo ) {
		if ( fileInfo.length == 1 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[0]->SetPoint(graph[0]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nAlpha) / refInfo.nAlpha);
		} else if ( fileInfo.length == 3 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[1]->SetPoint(graph[1]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nAlpha) / refInfo.nAlpha);
		} else if ( fileInfo.length == 7 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[2]->SetPoint(graph[2]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nAlpha) / refInfo.nAlpha);
		} else if ( fileInfo.length == 20 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[3]->SetPoint(graph[3]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nAlpha) / refInfo.nAlpha);
		}
	}

	TMultiGraph* multiGraph = new TMultiGraph();
	graph[0]->SetLineColor(kRed);
	graph[0]->SetLineWidth(2);
	legend->AddEntry(graph[0], "1mm", "l");
	multiGraph->Add(graph[0]);
	graph[1]->SetLineColor(kBlue);
	graph[1]->SetLineWidth(2);
	legend->AddEntry(graph[1], "3mm", "l");
	multiGraph->Add(graph[1]);
	graph[2]->SetLineColor(kMagenta);
	graph[2]->SetLineWidth(2);
	legend->AddEntry(graph[2], "7mm", "l");
	multiGraph->Add(graph[2]);
	graph[3]->SetLineColor(kGreen);
	graph[3]->SetLineWidth(2);
	legend->AddEntry(graph[3], "20mm", "l");
	multiGraph->Add(graph[3]);

	savePlot(canvas, multiGraph, mConfig.getConfig("NAlpha"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("NAlpha"));
}

void TGeantComparison::drawNElectronGraph() {
	TCanvas* canvas = new TCanvas("nElectronCanvas", "", 1000, 1000);
	TGraph* graph[4];
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	graph[0] = new TGraph();
	graph[1] = new TGraph();
	graph[2] = new TGraph();
	graph[3] = new TGraph();

	for ( const FileInfo& fileInfo : mFileInfo ) {
		if ( fileInfo.length == 1 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[0]->SetPoint(graph[0]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / refInfo.nElectron);
		} else if ( fileInfo.length == 3 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[1]->SetPoint(graph[1]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / refInfo.nElectron);
		} else if ( fileInfo.length == 7 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[2]->SetPoint(graph[2]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / refInfo.nElectron);
		} else if ( fileInfo.length == 20 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[3]->SetPoint(graph[3]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / refInfo.nElectron);
		}
	}

	TMultiGraph* multiGraph = new TMultiGraph();
	graph[0]->SetLineColor(kRed);
	graph[0]->SetLineWidth(2);
	legend->AddEntry(graph[0], "1mm", "l");
	multiGraph->Add(graph[0]);
	graph[1]->SetLineColor(kBlue);
	graph[1]->SetLineWidth(2);
	legend->AddEntry(graph[1], "3mm", "l");
	multiGraph->Add(graph[1]);
	graph[2]->SetLineColor(kMagenta);
	graph[2]->SetLineWidth(2);
	legend->AddEntry(graph[2], "7mm", "l");
	multiGraph->Add(graph[2]);
	graph[3]->SetLineColor(kGreen);
	graph[3]->SetLineWidth(2);
	legend->AddEntry(graph[3], "20mm", "l");
	multiGraph->Add(graph[3]);

	savePlot(canvas, multiGraph, mConfig.getConfig("NElectron"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("NElectron"));
}

void TGeantComparison::drawAlphaElectronRatioGraph() {
	TCanvas* canvas = new TCanvas("alphaElectronRatioCanvas", "", 1000, 1000);
	TGraph* graph[4];
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	graph[0] = new TGraph();
	graph[1] = new TGraph();
	graph[2] = new TGraph();
	graph[3] = new TGraph();

	for ( const FileInfo& fileInfo : mFileInfo ) {
		if ( fileInfo.length == 1 ) {
			graph[0]->SetPoint(graph[0]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / fileInfo.nAlpha);
		} else if ( fileInfo.length == 3 ) {
			graph[1]->SetPoint(graph[1]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / fileInfo.nAlpha);
		} else if ( fileInfo.length == 7 ) {
			graph[2]->SetPoint(graph[2]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / fileInfo.nAlpha);
		} else if ( fileInfo.length == 20 ) {
			graph[3]->SetPoint(graph[3]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / fileInfo.nAlpha);
		}
	}

	TMultiGraph* multiGraph = new TMultiGraph();
	graph[0]->SetLineColor(kRed);
	graph[0]->SetLineWidth(2);
	legend->AddEntry(graph[0], "1mm", "l");
	multiGraph->Add(graph[0]);
	graph[1]->SetLineColor(kBlue);
	graph[1]->SetLineWidth(2);
	legend->AddEntry(graph[1], "3mm", "l");
	multiGraph->Add(graph[1]);
	graph[2]->SetLineColor(kMagenta);
	graph[2]->SetLineWidth(2);
	legend->AddEntry(graph[2], "7mm", "l");
	multiGraph->Add(graph[2]);
	graph[3]->SetLineColor(kGreen);
	graph[3]->SetLineWidth(2);
	legend->AddEntry(graph[3], "20mm", "l");
	multiGraph->Add(graph[3]);

	savePlot(canvas, multiGraph, mConfig.getConfig("AlphaElectronRatio"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("AlphaElectronRatio"));
}

void TGeantComparison::drawNDoubleGraph() {
	TCanvas* canvas = new TCanvas("nDoubleCanvas", "", 1000, 1000);
	TGraph* graph[4];
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	graph[0] = new TGraph();
	graph[1] = new TGraph();
	graph[2] = new TGraph();
	graph[3] = new TGraph();

	for ( const FileInfo& fileInfo : mFileInfo ) {
		if ( fileInfo.length == 1 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[0]->SetPoint(graph[0]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / refInfo.nDouble);
		} else if ( fileInfo.length == 3 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[1]->SetPoint(graph[1]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / refInfo.nDouble);
		} else if ( fileInfo.length == 7 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[2]->SetPoint(graph[2]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / refInfo.nDouble);
		} else if ( fileInfo.length == 20 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[3]->SetPoint(graph[3]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / refInfo.nDouble);
		}
	}

	TMultiGraph* multiGraph = new TMultiGraph();
	graph[0]->SetLineColor(kRed);
	graph[0]->SetLineWidth(2);
	legend->AddEntry(graph[0], "1mm", "l");
	multiGraph->Add(graph[0]);
	graph[1]->SetLineColor(kBlue);
	graph[1]->SetLineWidth(2);
	legend->AddEntry(graph[1], "3mm", "l");
	multiGraph->Add(graph[1]);
	graph[2]->SetLineColor(kMagenta);
	graph[2]->SetLineWidth(2);
	legend->AddEntry(graph[2], "7mm", "l");
	multiGraph->Add(graph[2]);
	graph[3]->SetLineColor(kGreen);
	graph[3]->SetLineWidth(2);
	legend->AddEntry(graph[3], "20mm", "l");
	multiGraph->Add(graph[3]);

	savePlot(canvas, multiGraph, mConfig.getConfig("NDouble"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("NDouble"));
}

void TGeantComparison::drawDoubleSingleRatioGraph() {
	TCanvas* canvas = new TCanvas("doubleSingleRatioCanvas", "", 1000, 1000);
	TGraph* graph[4];
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	graph[0] = new TGraph();
	graph[1] = new TGraph();
	graph[2] = new TGraph();
	graph[3] = new TGraph();

	for ( const FileInfo& fileInfo : mFileInfo ) {
		if ( fileInfo.length == 1 ) {
			graph[0]->SetPoint(graph[0]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / (fileInfo.nAlpha - (2 * fileInfo.nDouble)));
		} else if ( fileInfo.length == 3 ) {
			graph[1]->SetPoint(graph[1]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / (fileInfo.nAlpha - (2 * fileInfo.nDouble)));
		} else if ( fileInfo.length == 7 ) {
			graph[2]->SetPoint(graph[2]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / (fileInfo.nAlpha - (2 * fileInfo.nDouble)));
		} else if ( fileInfo.length == 20 ) {
			graph[3]->SetPoint(graph[3]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / (fileInfo.nAlpha - (2 * fileInfo.nDouble)));
		}
	}

	TMultiGraph* multiGraph = new TMultiGraph();
	graph[0]->SetLineColor(kRed);
	graph[0]->SetLineWidth(2);
	legend->AddEntry(graph[0], "1mm", "l");
	multiGraph->Add(graph[0]);
	graph[1]->SetLineColor(kBlue);
	graph[1]->SetLineWidth(2);
	legend->AddEntry(graph[1], "3mm", "l");
	multiGraph->Add(graph[1]);
	graph[2]->SetLineColor(kMagenta);
	graph[2]->SetLineWidth(2);
	legend->AddEntry(graph[2], "7mm", "l");
	multiGraph->Add(graph[2]);
	graph[3]->SetLineColor(kGreen);
	graph[3]->SetLineWidth(2);
	legend->AddEntry(graph[3], "20mm", "l");
	multiGraph->Add(graph[3]);

	savePlot(canvas, multiGraph, mConfig.getConfig("DoubleSingleRatio"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("DoubleSingleRatio"));
}

void TGeantComparison::compareAlpha() {
	TCanvas* canvas = new TCanvas("alphaExperimentCanvas", "", 1000, 1000);
	TGraph* graph[4];
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	graph[0] = new TGraph();
	graph[1] = new TGraph();
	graph[2] = new TGraph();
	graph[3] = new TGraph();

	for ( const FileInfo& fileInfo : mFileInfo ) {
		if ( fileInfo.length == 1 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[0]->SetPoint(graph[0]->GetN(), fileInfo.area, (static_cast<Double_t>(fileInfo.nAlpha) - 2. * fileInfo.nDouble) / (refInfo.nAlpha - 2. * refInfo.nDouble));
		} else if ( fileInfo.length == 3 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[1]->SetPoint(graph[1]->GetN(), fileInfo.area, (static_cast<Double_t>(fileInfo.nAlpha) - 2. * fileInfo.nDouble) / (refInfo.nAlpha - 2. * refInfo.nDouble));
		} else if ( fileInfo.length == 7 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[2]->SetPoint(graph[2]->GetN(), fileInfo.area, (static_cast<Double_t>(fileInfo.nAlpha) - 2. * fileInfo.nDouble) / (refInfo.nAlpha - 2. * refInfo.nDouble));
		} else if ( fileInfo.length == 20 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[3]->SetPoint(graph[3]->GetN(), fileInfo.area, (static_cast<Double_t>(fileInfo.nAlpha) - 2. * fileInfo.nDouble) / (refInfo.nAlpha - 2. * refInfo.nDouble));
		}
	}

	TMultiGraph* multiGraph = new TMultiGraph();
	graph[0]->SetLineColor(kRed);
	graph[0]->SetLineWidth(2);
	legend->AddEntry(graph[0], "1mm Simulation", "l");
	multiGraph->Add(graph[0]);
	graph[1]->SetLineColor(kBlue);
	graph[1]->SetLineWidth(2);
	legend->AddEntry(graph[1], "3mm Simulation", "l");
	multiGraph->Add(graph[1]);
	graph[2]->SetLineColor(kMagenta);
	graph[2]->SetLineWidth(2);
	legend->AddEntry(graph[2], "7mm Simulation", "l");
	multiGraph->Add(graph[2]);
	graph[3]->SetLineColor(kGreen);
	graph[3]->SetLineWidth(2);
	legend->AddEntry(graph[3], "20mm Simulation", "l");
	multiGraph->Add(graph[3]);

	TGraphErrors* expGraph[4];
	expGraph[0] = new TGraphErrors();
	expGraph[1] = new TGraphErrors();
	expGraph[2] = new TGraphErrors();
	expGraph[3] = new TGraphErrors();

	for ( const ExpInfo& expInfo : mExpInfo ) {
		if ( expInfo.length == 1 ) {
			expGraph[0]->SetPoint(expGraph[0]->GetN(), expInfo.area, expInfo.regionC);
		} else if ( expInfo.length == 3 ) {
			expGraph[1]->SetPoint(expGraph[1]->GetN(), expInfo.area, expInfo.regionC);
		} else if ( expInfo.length == 7 ) {
			expGraph[2]->SetPoint(expGraph[2]->GetN(), expInfo.area, expInfo.regionC);
		} else if ( expInfo.length == 20 ) {
			expGraph[3]->SetPoint(expGraph[3]->GetN(), expInfo.area, expInfo.regionC);
		}
	}

	expGraph[0]->SetLineColor(kRed);
	expGraph[0]->SetLineWidth(2);
	expGraph[0]->SetLineStyle(10);
	legend->AddEntry(expGraph[0], "1mm Experiment", "l");
	multiGraph->Add(expGraph[0]);
	expGraph[1]->SetLineColor(kBlue);
	expGraph[1]->SetLineWidth(2);
	expGraph[1]->SetLineStyle(10);
	legend->AddEntry(expGraph[1], "3mm Experiment", "l");
	multiGraph->Add(expGraph[1]);
	expGraph[2]->SetLineColor(kMagenta);
	expGraph[2]->SetLineWidth(2);
	expGraph[2]->SetLineStyle(10);
	legend->AddEntry(expGraph[2], "7mm Experiment", "l");
	multiGraph->Add(expGraph[2]);
	expGraph[3]->SetLineColor(kGreen);
	expGraph[3]->SetLineWidth(2);
	expGraph[3]->SetLineStyle(10);
	legend->AddEntry(expGraph[3], "20mm Experiment", "l");
	multiGraph->Add(expGraph[3]);

	multiGraph->SetMaximum(1);
	multiGraph->SetMinimum(0);

	savePlot(canvas, multiGraph, mConfig.getConfig("ExperimentSimulationAlphaRatio"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("ExperimentSimulationAlphaRatio"));
}

void TGeantComparison::compareElectron() {
	TCanvas* canvas = new TCanvas("electronExperimentCanvas", "", 1000, 1000);
	TGraph* graph[4];
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	graph[0] = new TGraph();
	graph[1] = new TGraph();
	graph[2] = new TGraph();
	graph[3] = new TGraph();

	for ( const FileInfo& fileInfo : mFileInfo ) {
		if ( fileInfo.length == 1 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[0]->SetPoint(graph[0]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / refInfo.nElectron);
		} else if ( fileInfo.length == 3 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[1]->SetPoint(graph[1]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / refInfo.nElectron);
		} else if ( fileInfo.length == 7 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[2]->SetPoint(graph[2]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / refInfo.nElectron);
		} else if ( fileInfo.length == 20 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[3]->SetPoint(graph[3]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nElectron) / refInfo.nElectron);
		}
	}

	TMultiGraph* multiGraph = new TMultiGraph();
	graph[0]->SetLineColor(kRed);
	graph[0]->SetLineWidth(2);
	legend->AddEntry(graph[0], "1mm Simulation", "l");
	multiGraph->Add(graph[0]);
	graph[1]->SetLineColor(kBlue);
	graph[1]->SetLineWidth(2);
	legend->AddEntry(graph[1], "3mm Simulation", "l");
	multiGraph->Add(graph[1]);
	graph[2]->SetLineColor(kMagenta);
	graph[2]->SetLineWidth(2);
	legend->AddEntry(graph[2], "7mm Simulation", "l");
	multiGraph->Add(graph[2]);
	graph[3]->SetLineColor(kGreen);
	graph[3]->SetLineWidth(2);
	legend->AddEntry(graph[3], "20mm Simulation", "l");
	multiGraph->Add(graph[3]);

	TGraphErrors* expGraph[4];
	expGraph[0] = new TGraphErrors();
	expGraph[1] = new TGraphErrors();
	expGraph[2] = new TGraphErrors();
	expGraph[3] = new TGraphErrors();

	for ( const ExpInfo& expInfo : mExpInfo ) {
		if ( expInfo.length == 1 ) {
			expGraph[0]->SetPoint(expGraph[0]->GetN(), expInfo.area, expInfo.regionA);
		} else if ( expInfo.length == 3 ) {
			expGraph[1]->SetPoint(expGraph[1]->GetN(), expInfo.area, expInfo.regionA);
		} else if ( expInfo.length == 7 ) {
			expGraph[2]->SetPoint(expGraph[2]->GetN(), expInfo.area, expInfo.regionA);
		} else if ( expInfo.length == 20 ) {
			expGraph[3]->SetPoint(expGraph[3]->GetN(), expInfo.area, expInfo.regionA);
		}
	}

	expGraph[0]->SetLineColor(kRed);
	expGraph[0]->SetLineWidth(2);
	expGraph[0]->SetLineStyle(10);
	legend->AddEntry(expGraph[0], "1mm Experiment", "l");
	multiGraph->Add(expGraph[0]);
	expGraph[1]->SetLineColor(kBlue);
	expGraph[1]->SetLineWidth(2);
	expGraph[1]->SetLineStyle(10);
	legend->AddEntry(expGraph[1], "3mm Experiment", "l");
	multiGraph->Add(expGraph[1]);
	expGraph[2]->SetLineColor(kMagenta);
	expGraph[2]->SetLineWidth(2);
	expGraph[2]->SetLineStyle(10);
	legend->AddEntry(expGraph[2], "7mm Experiment", "l");
	multiGraph->Add(expGraph[2]);
	expGraph[3]->SetLineColor(kGreen);
	expGraph[3]->SetLineWidth(2);
	expGraph[3]->SetLineStyle(10);
	legend->AddEntry(expGraph[3], "20mm Experiment", "l");
	multiGraph->Add(expGraph[3]);

	multiGraph->SetMaximum(1);
	multiGraph->SetMinimum(0);

	savePlot(canvas, multiGraph, mConfig.getConfig("ExperimentSimulationElectronRatio"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("ExperimentSimulationElectronRatio"));

}


void TGeantComparison::compareDouble() {
	TCanvas* canvas = new TCanvas("doubleExperimentCanvas", "", 1000, 1000);
	TGraph* graph[4];
	TLegend* legend = new TLegend(0.1, 0.7, 0.4, 0.9);
	graph[0] = new TGraph();
	graph[1] = new TGraph();
	graph[2] = new TGraph();
	graph[3] = new TGraph();

	for ( const FileInfo& fileInfo : mFileInfo ) {
		if ( fileInfo.length == 1 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[0]->SetPoint(graph[0]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / refInfo.nDouble);
		} else if ( fileInfo.length == 3 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[1]->SetPoint(graph[1]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / refInfo.nDouble);
		} else if ( fileInfo.length == 7 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[2]->SetPoint(graph[2]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / refInfo.nDouble);
		} else if ( fileInfo.length == 20 ) {
			FileInfo refInfo;
			for ( const FileInfo& ref : mReference ) {
				if ( ref.length == fileInfo.length ) {
					refInfo = ref;
					break;
				}
			}
			graph[3]->SetPoint(graph[3]->GetN(), fileInfo.area, static_cast<Double_t>(fileInfo.nDouble) / refInfo.nDouble);
		}
	}

	TMultiGraph* multiGraph = new TMultiGraph();
	graph[0]->SetLineColor(kRed);
	graph[0]->SetLineWidth(2);
	legend->AddEntry(graph[0], "1mm Simulation", "l");
	multiGraph->Add(graph[0]);
	graph[1]->SetLineColor(kBlue);
	graph[1]->SetLineWidth(2);
	legend->AddEntry(graph[1], "3mm Simulation", "l");
	multiGraph->Add(graph[1]);
	graph[2]->SetLineColor(kMagenta);
	graph[2]->SetLineWidth(2);
	legend->AddEntry(graph[2], "7mm Simulation", "l");
	multiGraph->Add(graph[2]);
	graph[3]->SetLineColor(kGreen);
	graph[3]->SetLineWidth(2);
	legend->AddEntry(graph[3], "20mm Simulation", "l");
	multiGraph->Add(graph[3]);

	TGraphErrors* expGraph[4];
	expGraph[0] = new TGraphErrors();
	expGraph[1] = new TGraphErrors();
	expGraph[2] = new TGraphErrors();
	expGraph[3] = new TGraphErrors();

	for ( const ExpInfo& expInfo : mExpInfo ) {
		if ( expInfo.length == 1 ) {
			expGraph[0]->SetPoint(expGraph[0]->GetN(), expInfo.area, expInfo.regionD);
		} else if ( expInfo.length == 3 ) {
			expGraph[1]->SetPoint(expGraph[1]->GetN(), expInfo.area, expInfo.regionD);
		} else if ( expInfo.length == 7 ) {
			expGraph[2]->SetPoint(expGraph[2]->GetN(), expInfo.area, expInfo.regionD);
		} else if ( expInfo.length == 20 ) {
			expGraph[3]->SetPoint(expGraph[3]->GetN(), expInfo.area, expInfo.regionD);
		}
	}

	expGraph[0]->SetLineColor(kRed);
	expGraph[0]->SetLineWidth(2);
	expGraph[0]->SetLineStyle(10);
	legend->AddEntry(expGraph[0], "1mm Experiment", "l");
	multiGraph->Add(expGraph[0]);
	expGraph[1]->SetLineColor(kBlue);
	expGraph[1]->SetLineWidth(2);
	expGraph[1]->SetLineStyle(10);
	legend->AddEntry(expGraph[1], "3mm Experiment", "l");
	multiGraph->Add(expGraph[1]);
	expGraph[2]->SetLineColor(kMagenta);
	expGraph[2]->SetLineWidth(2);
	expGraph[2]->SetLineStyle(10);
	legend->AddEntry(expGraph[2], "7mm Experiment", "l");
	multiGraph->Add(expGraph[2]);
	expGraph[3]->SetLineColor(kGreen);
	expGraph[3]->SetLineWidth(2);
	expGraph[3]->SetLineStyle(10);
	legend->AddEntry(expGraph[3], "20mm Experiment", "l");
	multiGraph->Add(expGraph[3]);

	multiGraph->SetMaximum(1);
	multiGraph->SetMinimum(0);

	savePlot(canvas, multiGraph, mConfig.getConfig("ExperimentSimulationDoubleSingleRatio"));
	saveLegend(canvas, legend);
	saveCanvas(canvas, mOutputPath, mConfig.getConfig("ExperimentSimulationDoubleSingleRatio"));
}