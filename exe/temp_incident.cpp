#include<iostream>
#include<filesystem>
#include<csv.h>
#include<vector>

#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"

int main(int argc, char* argv) {
	std::filesystem::path csvPath = "../Data/incident_angle.csv";

	io::CSVReader<8> csv(csvPath);

	csv.read_header(io::ignore_extra_column, "collimator_length", "collimator_phi", "collimator_area", "collimator_area_error", "incident_angle_mean", "incident_angle_stdev", "num_electron", "num_alpha");

	int collimator_length = 0;
	int collimator_phi = 0;
	double collimator_area = 0.;
	double collimator_area_error = 0.;
	double incident_angle_mean = 0.;
	double incident_angle_stdev = 0.;
	double num_electron = 0;
	double num_alpha = 0;

	TGraphErrors* incidentAngleGraphSet[4];
	incidentAngleGraphSet[0] = new TGraphErrors();
	incidentAngleGraphSet[1] = new TGraphErrors();
	incidentAngleGraphSet[2] = new TGraphErrors();
	incidentAngleGraphSet[3] = new TGraphErrors();

	TGraph* electronGraphSet[4];
	electronGraphSet[0] = new TGraph();
	electronGraphSet[1] = new TGraph();
	electronGraphSet[2] = new TGraph();
	electronGraphSet[3] = new TGraph();

	TGraph* particleRatioGraphSet[4];
	particleRatioGraphSet[0] = new TGraph();
	particleRatioGraphSet[1] = new TGraph();
	particleRatioGraphSet[2] = new TGraph();
	particleRatioGraphSet[3] = new TGraph();

	while ( csv.read_row(collimator_length, collimator_phi, collimator_area, collimator_area_error, incident_angle_mean, incident_angle_stdev, num_electron, num_alpha) ) {
		if ( collimator_length == 1 ) {
			incidentAngleGraphSet[0]->SetPoint(incidentAngleGraphSet[0]->GetN(), collimator_area, incident_angle_mean);
			incidentAngleGraphSet[0]->SetPointError(incidentAngleGraphSet[0]->GetN() - 1, collimator_area_error, incident_angle_stdev);
			electronGraphSet[0]->SetPoint(electronGraphSet[0]->GetN(), collimator_area, num_electron);
			particleRatioGraphSet[0]->SetPoint(particleRatioGraphSet[0]->GetN(), collimator_area, num_electron / num_alpha);
		} else if ( collimator_length == 3 ) {
			incidentAngleGraphSet[1]->SetPoint(incidentAngleGraphSet[1]->GetN(), collimator_area, incident_angle_mean);
			incidentAngleGraphSet[1]->SetPointError(incidentAngleGraphSet[1]->GetN() - 1, collimator_area_error, incident_angle_stdev);
			electronGraphSet[1]->SetPoint(electronGraphSet[1]->GetN(), collimator_area, num_electron);
			particleRatioGraphSet[1]->SetPoint(particleRatioGraphSet[1]->GetN(), collimator_area, num_electron / num_alpha);
		} else if ( collimator_length == 7 ) {
			incidentAngleGraphSet[2]->SetPoint(incidentAngleGraphSet[2]->GetN(), collimator_area, incident_angle_mean);
			incidentAngleGraphSet[2]->SetPointError(incidentAngleGraphSet[2]->GetN() - 1, collimator_area_error, incident_angle_stdev);
			electronGraphSet[2]->SetPoint(electronGraphSet[2]->GetN(), collimator_area, num_electron);
			particleRatioGraphSet[2]->SetPoint(particleRatioGraphSet[2]->GetN(), collimator_area, num_electron / num_alpha);
		} else if ( collimator_length == 20 ) {
			incidentAngleGraphSet[3]->SetPoint(incidentAngleGraphSet[3]->GetN(), collimator_area, incident_angle_mean);
			incidentAngleGraphSet[3]->SetPointError(incidentAngleGraphSet[3]->GetN() - 1, collimator_area_error, incident_angle_stdev);
			electronGraphSet[3]->SetPoint(electronGraphSet[3]->GetN(), collimator_area, num_electron);
			particleRatioGraphSet[3]->SetPoint(particleRatioGraphSet[3]->GetN(), collimator_area, num_electron / num_alpha);
		}
	}
	TMultiGraph* incidentAngleMultiGraph = new TMultiGraph();
	incidentAngleGraphSet[0]->SetLineColor(kRed);
	incidentAngleGraphSet[0]->SetLineWidth(2);
	incidentAngleMultiGraph->Add(incidentAngleGraphSet[0]);
	incidentAngleGraphSet[1]->SetLineColor(kGreen);
	incidentAngleGraphSet[1]->SetLineWidth(2);
	incidentAngleMultiGraph->Add(incidentAngleGraphSet[1]);
	incidentAngleGraphSet[2]->SetLineColor(kMagenta);
	incidentAngleGraphSet[2]->SetLineWidth(2);
	incidentAngleMultiGraph->Add(incidentAngleGraphSet[2]);
	incidentAngleGraphSet[3]->SetLineColor(kBlue);
	incidentAngleGraphSet[3]->SetLineWidth(2);
	incidentAngleMultiGraph->Add(incidentAngleGraphSet[3]);

	TCanvas* canvas = new TCanvas("canvas", "", 2000, 1000);
	incidentAngleMultiGraph->SetMaximum(90);
	incidentAngleMultiGraph->SetMinimum(0);
	incidentAngleMultiGraph->SetTitle("Incident Angle; Collimator Area [mm^{2}]; Incident Angle [degree]");

	TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
	legend->SetHeader("Collimator Length", "C");
	legend->AddEntry(incidentAngleGraphSet[0], "1 mm", "l");
	legend->AddEntry(incidentAngleGraphSet[1], "3 mm", "l");
	legend->AddEntry(incidentAngleGraphSet[2], "7 mm", "l");
	legend->AddEntry(incidentAngleGraphSet[3], "20 mm", "l");

	incidentAngleMultiGraph->Draw("AL");
	legend->Draw("SAME");

	canvas->SaveAs("/mnt/homes/ychoi/entry_test/geant4/incident_angle.png");

	TMultiGraph* electronMultiGraph = new TMultiGraph();
	electronGraphSet[0]->SetLineColor(kRed);
	electronGraphSet[0]->SetLineWidth(2);
	electronMultiGraph->Add(electronGraphSet[0]);
	electronGraphSet[1]->SetLineColor(kGreen);
	electronGraphSet[1]->SetLineWidth(2);
	electronMultiGraph->Add(electronGraphSet[1]);
	electronGraphSet[2]->SetLineColor(kMagenta);
	electronGraphSet[2]->SetLineWidth(2);
	electronMultiGraph->Add(electronGraphSet[2]);
	electronGraphSet[3]->SetLineColor(kBlue);
	electronGraphSet[3]->SetLineWidth(2);
	electronMultiGraph->Add(electronGraphSet[3]);

	TCanvas* electronCanvas = new TCanvas("electronCanvas", "", 2000, 1000);
	electronMultiGraph->SetTitle("Number of Electron; Collimator Area [mm^{2}]; Number of Electron");

	TLegend* electronLegend = new TLegend(0.1, 0.7, 0.3, 0.9);
	electronLegend->SetHeader("Collimator Length", "C");
	electronLegend->AddEntry(electronGraphSet[0], "1 mm", "l");
	electronLegend->AddEntry(electronGraphSet[1], "3 mm", "l");
	electronLegend->AddEntry(electronGraphSet[2], "7 mm", "l");
	electronLegend->AddEntry(electronGraphSet[3], "20 mm", "l");

	electronMultiGraph->Draw("AL");
	electronLegend->Draw("SAME");

	electronCanvas->SaveAs("/mnt/homes/ychoi/entry_test/geant4/num_electron.png");

	TMultiGraph* particleRatioMultiGraph = new TMultiGraph();
	particleRatioGraphSet[0]->SetLineColor(kRed);
	particleRatioGraphSet[0]->SetLineWidth(2);
	particleRatioMultiGraph->Add(particleRatioGraphSet[0]);
	particleRatioGraphSet[1]->SetLineColor(kGreen);
	particleRatioGraphSet[1]->SetLineWidth(2);
	particleRatioMultiGraph->Add(particleRatioGraphSet[1]);
	particleRatioGraphSet[2]->SetLineColor(kMagenta);
	particleRatioGraphSet[2]->SetLineWidth(2);
	particleRatioMultiGraph->Add(particleRatioGraphSet[2]);
	particleRatioGraphSet[3]->SetLineColor(kBlue);
	particleRatioGraphSet[3]->SetLineWidth(2);
	particleRatioMultiGraph->Add(particleRatioGraphSet[3]);

	TCanvas* particleRatioCanvas = new TCanvas("particleRatioCanvas", "", 2000, 1000);
	particleRatioMultiGraph->SetMaximum(.6);
	particleRatioMultiGraph->SetMinimum(0);
	particleRatioMultiGraph->SetTitle("Ratio of Electron to Alpha; Collimator Area [mm^{2}]; # of Electron / # of Alpha");

	TLegend* particleRatioLegend = new TLegend(0.7, 0.7, 0.9, 0.9);
	particleRatioLegend->SetHeader("Collimator Length", "C");
	particleRatioLegend->AddEntry(particleRatioGraphSet[0], "1 mm", "l");
	particleRatioLegend->AddEntry(particleRatioGraphSet[1], "3 mm", "l");
	particleRatioLegend->AddEntry(particleRatioGraphSet[2], "7 mm", "l");
	particleRatioLegend->AddEntry(particleRatioGraphSet[3], "20 mm", "l");

	particleRatioMultiGraph->Draw("AL");
	particleRatioLegend->Draw("SAME");

	particleRatioCanvas->SaveAs("/mnt/homes/ychoi/entry_test/geant4/particle_ratio.png");

	return 0;
}