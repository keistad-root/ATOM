#define __TGRAPHCOMPARE_HEADER__
#include "TGraphCompare.h"

TH1D* TGraphCompare::getClustersizeHistogram(std::string_view pathInRoot, std::string operation) {
	std::vector<std::string> stringSet;

	size_t start = 0, end = 0;
	operation.erase(remove(operation.begin(), operation.end(), ' '), operation.end());
	while ( (end = operation.find_first_of("+-*/", start)) != std::string::npos ) {
		if ( end != start ) {
			stringSet.push_back(operation.substr(start, end - start));
		}
		stringSet.push_back(std::string(1, operation[end]));
		start = end + 1;
	}
	if ( start < operation.length() ) {
		stringSet.push_back(operation.substr(start));
	}

	TH1D* hist1 = (TH1D*) mGraphFileSet.find(stringSet[0])->second->Get(static_cast<TString>(pathInRoot));
	TH1D* newHist = new TH1D(*hist1);
	if ( stringSet.size() == 3 ) {
		TH1D* hist2 = (TH1D*) mGraphFileSet.find(stringSet[2])->second->Get(static_cast<TString>(pathInRoot));
		if ( stringSet[1] == "+" ) {
			newHist->Add(hist2);
			for ( int i = 0; i < 80; i++ ) {
				newHist->SetBinError(i, sqrt(hist1->GetBinContent(i) + hist2->GetBinContent(i)));
			}
		} else if ( stringSet[1] == "-" ) {
			newHist->Add(hist2, -1);
			for ( int i = 0; i < 80; i++ ) {
				newHist->SetBinError(i, sqrt(hist1->GetBinContent(i) + hist2->GetBinContent(i)));
			}
		} else if ( stringSet[1] == "*" ) {
			newHist->Multiply(hist2);
			for ( int i = 0; i < 80; i++ ) {
				newHist->SetBinError(i, sqrt(hist1->GetBinContent(i) * hist2->GetBinContent(i) * (hist1->GetBinContent(i) + hist2->GetBinContent(i))));
			}
		} else if ( stringSet[1] == "/" ) {
			newHist->Divide(hist2);
			for ( int i = 0; i < 80; i++ ) {
				newHist->SetBinError(i, sqrt(pow(hist1->GetBinContent(i) / hist2->GetBinContent(i), 4) + 1));
			}
		}
	}
	return newHist;
}

int getColor(const std::string colorName) {
	if ( colorName == "kRed" ) {
		return kRed;
	} else if ( colorName == "kMagenta" ) {
		return kMagenta;
	} else if ( colorName == "kBlue" ) {
		return kBlue;
	} else if ( colorName == "kCyan" ) {
		return kCyan + 2;
	} else if ( colorName == "kOrange" ) {
		return kOrange;
	} else {
		return kWhite;
	}
}

int getStyle(const int index) {
	if ( index == 0 ) {
		return 1;
	} else if ( index == 1 ) {
		return 8;
	} else if ( index == 2 ) {
		return 10;
	} else if ( index == 3 ) {
		return 6;
	} else if ( index == 4 ) {
		return 9;
	} else {
		return 0;
	}
}

TGraphCompare::TGraphCompare(const std::vector<std::string>& graphFilePath) {
	for ( std::string_view filePath : graphFilePath ) {
		std::filesystem::path path(filePath);
		TFile* file = new TFile(static_cast<TString>(path));
		mGraphFileSet.insert_or_assign(std::string(path.stem()), file);
	}
}

void TGraphCompare::TCompareClusterSize(std::string_view typeName, const CppConfigDictionary config) {
	TString canvasName = "can_" + config.find("name");
	Int_t canvasWidth = stoi(config.getSubConfig("canvas").find("width"));
	Int_t canvasHeight = stoi(config.getSubConfig("canvas").find("height"));
	TCanvas* canvas = new TCanvas(canvasName, "", canvasWidth, canvasHeight);

	TString legendTitle = config.getSubConfig("legend").hasKey("title") ? config.getSubConfig("legend").find("title") : "";
	Float_t legendXmin = config.getSubConfig("legend").hasKey("x_min") ? stof(config.getSubConfig("legend").find("x_min")) : 0.7;
	Float_t legendXmax = config.getSubConfig("legend").hasKey("x_max") ? stof(config.getSubConfig("legend").find("x_max")) : 0.7;
	Float_t legendYmin = config.getSubConfig("legend").hasKey("y_min") ? stof(config.getSubConfig("legend").find("y_min")) : 0.9;
	Float_t legendYmax = config.getSubConfig("legend").hasKey("y_max") ? stof(config.getSubConfig("legend").find("y_max")) : 0.9;
	Float_t legendDivide = config.getSubConfig("legend").hasKey("divide") ? stoi(config.getSubConfig("legend").find("divide")) : 1;
	TLegend* legend = new TLegend(legendXmin, legendYmin, legendXmax, legendYmax);
	legend->SetHeader(legendTitle, "c");
	legend->SetNColumns(legendDivide);
	std::map<std::string, CppConfigDictionary> plotConfigList;
	for ( const auto& pair : config.getSubConfig("plots").getSubConfigSetWithName() ) {
		plotConfigList.insert_or_assign(pair.first, pair.second);
	}
	std::map<std::string, TH1D*> distributionSet;

	std::string rootPath = config.find("root_path");
	for ( const auto& plotConfig : plotConfigList ) {
		distributionSet.insert_or_assign(plotConfig.first, getClustersizeHistogram(rootPath, plotConfig.second.find("histogram")));
	}

	bool isFirst;
	for ( const auto& distribution : distributionSet ) {
		if ( isFirst ) {

			isFirst = false;
		}

		double nEntry = 0;
		int csMin = stoi(config.find("cluster_size_of_interest_min"));
		int csMax = stoi(config.find("cluster_size_of_interest_max"));

		for ( int clusterSize = 1; clusterSize < 81; clusterSize++ ) {
			if ( clusterSize < csMin || clusterSize > csMax ) {
				distribution.second->SetBinContent(clusterSize, 0);
			}
		}

		if ( config.hasKey("normalization") && (config.find("normalization") == "true") ) {
			distribution.second->Scale(1. / nEntry);
		} else if ( plotConfigList.find(distribution.first)->second.hasKey("scale") ) {
			Float_t scaleFactor = stof(plotConfigList.find(distribution.first)->second.find("scale"));
			distribution.second->Scale(scaleFactor);
		}


		for ( int clusterSize = csMin; clusterSize < csMax + 1; clusterSize++ ) {
			nEntry += distribution.second->GetBinContent(clusterSize);
		}
		TString legendTitle = plotConfigList.find(distribution.first)->second.find("legend_title");
		TString strEntry = Form("%.2f", round(nEntry * 100) / 100.);
		if ( plotConfigList.find(distribution.first)->second.hasKey("scale_error_max") ) {
			TString strEntryPlus = Form("%.2f", abs(round(nEntry * (stof(plotConfigList.find(distribution.first)->second.find("scale_error_max")) - 1) * 100) / 100.));
			TString strEntryMinus = Form("%.2f", abs(round(nEntry * (1 - stof(plotConfigList.find(distribution.first)->second.find("scale_error_min"))) * 100) / 100.));
			legend->AddEntry(distribution.second, legendTitle + "(" + strEntry + "+" + strEntryPlus + "-" + strEntryMinus + " in " + std::to_string(csMin) + " ~ " + std::to_string(csMax) + ")", "l");
		} else {
			Float_t scaleFactor = stof(plotConfigList.find(distribution.first)->second.find("scale"));
			TString strError = Form("%.2f", sqrt(nEntry * scaleFactor));
			legend->AddEntry(distribution.second, legendTitle + "(" + strEntry + char(0x00B1) + strError + " in " + std::to_string(csMin) + " ~ " + std::to_string(csMax) + ")", "l");
		}

		TString title = config.find("title");
		TString xTitle = config.find("x_title");
		TString yTitle = config.find("y_title");

		distribution.second->SetMinimum(stof(config.find("y_min")));
		distribution.second->SetMaximum(stof(config.find("y_max")));
		distribution.second->GetXaxis()->SetRangeUser(csMin - .5, csMax + .5);
		distribution.second->SetTitle(title + "; " + xTitle + "; " + yTitle);
		distribution.second->SetLineColor(getColor(plotConfigList.find(distribution.first)->second.find("line_color")));

		Float_t lineWidth = config.hasKey("line_width") ? stof(config.find("line_width")) : 1.;
		distribution.second->SetLineWidth(lineWidth);
		Float_t lineStyle = plotConfigList.find(distribution.first)->second.hasKey("line_style") ? stof(plotConfigList.find(distribution.first)->second.find("line_style")) : 1.;
		distribution.second->SetLineStyle(lineStyle);

		distribution.second->SetStats(0);
		distribution.second->Draw("SAME HISTE");
		if ( plotConfigList.find(distribution.first)->second.hasKey("scale_error_max") ) {
			TGraphAsymmErrors* systemGraph = new TGraphAsymmErrors();
			for ( int clusterSize = csMin; clusterSize < csMax + 1; clusterSize++ ) {
				systemGraph->SetPoint(clusterSize, clusterSize, distribution.second->GetBinContent(clusterSize));
				systemGraph->SetPointError(clusterSize, .5, .5, distribution.second->GetBinContent(clusterSize) * (1 - stof(plotConfigList.find(distribution.first)->second.find("scale_error_min"))), distribution.second->GetBinContent(clusterSize) * (stof(plotConfigList.find(distribution.first)->second.find("scale_error_max")) - 1));
			}
			Float_t alpha = stof(config.find("error_box_alpha"));
			systemGraph->SetFillColorAlpha(getColor(plotConfigList.find(distribution.first)->second.find("line_color")), alpha);
			systemGraph->Draw("SAME E2");
		}
	}

	legend->Draw();
	if ( config.hasKey("logy") && (config.find("logy") == "true") ) {
		canvas->SetLogy();
	}
	std::filesystem::path outputPath(config.find("output_path"));
	std::filesystem::create_directories(outputPath);
	outputPath /= config.find("name");
	outputPath.replace_extension(config.find("extension"));

	canvas->SaveAs(static_cast<TString>(outputPath));
}