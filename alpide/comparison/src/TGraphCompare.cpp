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
		Float_t scaleFactor = stof(plotConfigList.find(distribution.first)->second.find("scale"));
		distribution.second->Scale(scaleFactor);

		int nEntry = 0;
		int csMin = stoi(config.find("cluster_size_of_interest_min"));
		int csMax = stoi(config.find("cluster_size_of_interest_max"));
		for ( int clusterSize = csMin; clusterSize < csMax + 1; clusterSize++ ) {
			nEntry += distribution.second->GetBinContent(clusterSize);
		}
		for ( int clusterSize = 1; clusterSize < 81; clusterSize++ ) {
			if ( clusterSize < csMin || clusterSize > csMax ) {
				distribution.second->SetBinContent(clusterSize, 0);
			}
		}

		TString legendTitle = plotConfigList.find(distribution.first)->second.find("legend_title");
		legend->AddEntry(distribution.second, legendTitle + "(" + std::to_string(nEntry) + " in " + std::to_string(csMin) + " ~ " + std::to_string(csMax) + ")", "l");

		TString title = config.find("title");
		TString xTitle = config.find("x_title");
		TString yTitle = config.find("y_title");

		distribution.second->SetMinimum(stof(config.find("y_min")));
		distribution.second->SetMaximum(stof(config.find("y_max")));
		distribution.second->GetXaxis()->SetRangeUser(csMin - 1.5, csMax + .5);
		distribution.second->SetTitle(title + "; " + xTitle + "; " + yTitle);
		distribution.second->SetLineColor(getColor(plotConfigList.find(distribution.first)->second.find("line_color")));
		distribution.second->SetStats(0);
		distribution.second->Draw("SAME HISTE");
	}

	legend->Draw();
	canvas->SetLogy();
	std::filesystem::path outputPath(config.find("output_path"));
	outputPath /= config.find("name");
	outputPath.replace_extension(config.find("extension"));

	canvas->SaveAs(static_cast<TString>(outputPath));
}

// void TGraphCompare::TCompareClusterSizeRatio(std::string_view typeName, const CppConfigDictionary config) {
// 	TString canvasName = "can_mcs";


// 	TCanvas* canvas = new TCanvas("Multi Cluster Size", "", 2000, 1000);
// 	int colorIndex = 0;
// 	TLegend* legend = new TLegend(0.5, 0.6, 0.9, 0.9);
// 	legend->SetHeader(static_cast<TString>(config.find("legend")), "c");
// 	int legend_divide = stoi(config.find("legend_divide"));
// 	legend->SetNColumns(legend_divide);
// 	double maximum = 0;
// 	std::map<std::string, TH1D*> histSet;
// 	TH1D* firstHist;
// 	TH1D* criteriaHist;
// 	bool isFirst = true;
// 	for ( TGraphObjects& graphObject : mGraphObjectSet ) {
// 		histSet.insert_or_assign(graphObject.name, graphObject.getClustersizeHistogram(std::string(typeName) + "/clustersize", mGraphFileSet));
// 	}
// 	for ( const auto& clusterSize : histSet ) {
// 		if ( isFirst ) {
// 			criteriaHist = clusterSize.second;
// 		}
// 		TH1D* hist1 = clusterSize.second;
// 		TH1D* hist = new TH1D(*hist1);
// 		if ( isFirst ) {
// 			firstHist = hist;
// 			isFirst = false;
// 		}
// 		hist->Divide(criteriaHist);
// 		hist->SetTitle(static_cast<TString>(config.find("plot_title")));
// 		for ( TGraphObjects& graphObject : mGraphObjectSet ) {
// 			if ( graphObject.name == clusterSize.first ) {
// 				hist->Scale(graphObject.weighting);
// 			}
// 		}
// 		hist->SetStats(0);
// 		maximum = hist->GetMaximum() > maximum ? hist->GetMaximum() : maximum;
// 		int entry = 0;
// 		for ( int i = 0; i < 35; i++ ) {
// 			entry += hist->GetBinContent(i);
// 		}
// 		for ( TGraphObjects& graphObject : mGraphObjectSet ) {
// 			if ( graphObject.name == clusterSize.first ) {
// 				legend->AddEntry(hist, static_cast<TString>(graphObject.legend + "(" + std::to_string(entry) + " in 0 ~ 35)"), "l");
// 			}
// 		}
// 		hist->SetLineColorAlpha(getColor(colorIndex), .9);
// 		hist->SetLineWidth(2);
// 		hist->Draw("SAME");
// 		colorIndex++;
// 	}
// 	firstHist->SetMaximum(1000);
// 	firstHist->SetMinimum(0.1);
// 	legend->Draw();
// 	canvas->SetLogy();
// 	std::filesystem::path outputPath(config.find("file_path"));
// 	canvas->SaveAs(static_cast<TString>(outputPath));
// }

// void TGraphCompare::TCompareClusterSize3D(std::string_view typeName, const CppConfigDictionary config) {
// 	TCanvas* canvas = new TCanvas("mcs3D", "", 3000, 2000);
// 	int nBinX = mClusterSizeSet.size();
// 	TH2D* clusterSizeCompare = new TH2D("clusterSizeCompare", "Cluster Size Distributions", nBinX, -nBinX - 2.5, -2.5, 60, -60.5, -0.5);
// 	int index = 0;
// 	for ( const auto& clusterSize : mClusterSizeSet ) {
// 		const TH1D* distribution = clusterSize;
// 		for ( int i = 0; i < distribution->GetEntries(); i++ ) {
// 			clusterSizeCompare->Fill(-stoi(getGraphInfo(std::string(mGraphInfoSet[index]), config.find("legend"))), -i, distribution->GetBinContent(i));
// 		}
// 		index++;
// 	}
// 	for ( int iXbin = 1; iXbin < clusterSizeCompare->GetNbinsX(); ++iXbin ) {
// 		clusterSizeCompare->GetXaxis()->SetBinLabel(iXbin, Form("%g", floor(-1 * clusterSizeCompare->GetXaxis()->GetBinLowEdge(iXbin))));
// 	}
// 	for ( int iYbin = 1; iYbin < clusterSizeCompare->GetNbinsY(); ++(++iYbin) ) {
// 		clusterSizeCompare->GetYaxis()->SetBinLabel(iYbin, Form("%g", floor(-1 * clusterSizeCompare->GetYaxis()->GetBinLowEdge(iYbin))));
// 	}
// 	clusterSizeCompare->SetTitle(static_cast<TString>(config.find("plot_title")));
// 	clusterSizeCompare->Draw("LEGO2");
// 	clusterSizeCompare->SetStats(0);
// 	canvas->SetPhi(85);
// 	canvas->SetTheta(40);
// 	canvas->SetLogz();
// 	std::filesystem::path outputPath(config.find("file_path"));
// 	canvas->SaveAs(static_cast<TString>(outputPath));
// }