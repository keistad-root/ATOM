#include "TEntrySimulation.h"

#include "TGraph.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TF1.h"

std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>>& matrix) {
	std::vector<std::vector<double>> returnMatrix;
	if ( matrix[0][0] == 0 ) {
		std::cout << "Error" << std::endl;
	} else {
		int nRow = matrix.size();
		int nColumn = matrix[0].size();

		std::vector<std::vector<double>> tempMatrix;
		for ( int iRow = 0; iRow < nRow; iRow++ ) {
			std::vector<double> temp;
			for ( int iColumn = 0; iColumn < 2 * nColumn; iColumn++ ) {
				if ( iColumn < nColumn ) {
					temp.push_back(matrix[iRow][iColumn]);
				} else {
					if ( iColumn - nColumn == iRow ) {
						temp.push_back(1);
					} else {
						temp.push_back(0);
					}
				}
			}
			tempMatrix.push_back(temp);
		}
		for ( int i = 0; i < nRow; i++ ) {
			for ( int iRow = i + 1; iRow < nRow; iRow++ ) {
				std::vector<double> temp = tempMatrix[i];
				double factor1 = tempMatrix[iRow][i] / temp[i];
				for ( int iColumn = 0; iColumn < 2 * nColumn; iColumn++ ) {
					temp[iColumn] = temp[iColumn] * factor1;
				}
				for ( int iColumn = 0; iColumn < 2 * nColumn; iColumn++ ) {
					tempMatrix[iRow][iColumn] -= temp[iColumn];
				}
				double factor2 = tempMatrix[iRow][iRow];
				for ( int iColumn = 0; iColumn < 2 * nColumn; iColumn++ ) {
					tempMatrix[iRow][iColumn] /= factor2;
				}
			}
		}

		for ( int i = nRow - 1; i > -1; i-- ) {
			for ( int iRow = i - 1; iRow > -1; iRow-- ) {
				std::vector<double>temp = tempMatrix[i];
				double factor1 = tempMatrix[iRow][i] / temp[i];
				for ( int iColumn = 0; iColumn < 2 * nColumn; iColumn++ ) {
					temp[iColumn] = temp[iColumn] * factor1;
				}
				for ( int iColumn = 0; iColumn < 2 * nColumn; iColumn++ ) {
					tempMatrix[iRow][iColumn] -= temp[iColumn];
				}
				double factor2 = tempMatrix[iRow][iRow];
				for ( int iColumn = 0; iColumn < 2 * nColumn; iColumn++ ) {
					tempMatrix[iRow][iColumn] /= factor2;
				}
			}
		}
		for ( int iRow = 0; iRow < nRow; iRow++ ) {
			std::vector<double> temp;
			for ( int iColumn = 0; iColumn < nColumn; iColumn++ ) {
				temp.push_back(tempMatrix[iRow][iColumn + nColumn]);
			}
			returnMatrix.push_back(temp);
		}
	}

	for ( int iRow = 0; iRow < matrix.size(); iRow++ ) {
		for ( int iColumn = 0; iColumn < matrix.size(); iColumn++ ) {
			std::cout << matrix[iRow][iColumn] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for ( int iRow = 0; iRow < returnMatrix.size(); iRow++ ) {
		for ( int iColumn = 0; iColumn < returnMatrix.size(); iColumn++ ) {
			std::cout << returnMatrix[iRow][iColumn] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return returnMatrix;
}

void differentiate(const std::vector<std::array<double, 2>>& domain, std::vector<std::array<double, 2>>& range, int order = 1) {
	int domainSize = domain.size();

	for ( int iDomain = 0; iDomain < domainSize; iDomain++ ) {
		double dx = 0;
		double dy = 0;
		for ( int iX = iDomain - order; iX < iDomain + order + 1; iX++ ) {
			if ( iX < 0 || iX > domainSize - 1 ) {
				continue;
			}
			if ( iX < iDomain ) {
				dx += domain[iDomain][0] - domain[iX][0];
				dy += domain[iDomain][1] - domain[iX][1];
			} else if ( iX > iDomain ) {
				dx += domain[iX][0] - domain[iDomain][0];
				dy += domain[iX][1] - domain[iDomain][1];
			}
		}
		std::array<double, 2> temp = {domain[iDomain][0], dy / dx};
		range.push_back(temp);
	}
}

double interpolate(double x, const std::vector<std::array<double, 2>>& domain, int order = 1) {
	int domainSize = domain.size();
	int standardIndex = 0;
	for ( int iDomain = 0; iDomain < domainSize; iDomain++ ) {
		if ( domain[iDomain][0] > x ) {
			standardIndex = iDomain;
			break;
		}
		if ( iDomain == 0 && domain[iDomain][0] > x ) {
			standardIndex = -1;
			break;
		}
		if ( iDomain > domainSize - 1 ) {
			standardIndex = -1;
			break;
		}
	}
	if ( standardIndex == -1 ) {
		return 0;
	} else {
		// int nPre = floor((order + 1) / 2);
		// int nPost = order - nPre;
		int nPre = order;
		int nPost = order;

		if ( (standardIndex - nPre) < 0 ) {
			nPre = standardIndex;
			nPost = order - nPre;
		}
		if ( (standardIndex + nPost) > domainSize - 1 ) {
			nPost = standardIndex - domainSize + 1;
		}
		TGraph* graph = new TGraph();

		for ( int i = standardIndex - nPre; i < standardIndex + nPost + 1; i++ ) {
			graph->SetPoint(i - standardIndex + nPost, domain[i][0], domain[i][1]);

		}
		TF1* func;
		if ( order == 1 ) {
			func = new TF1("interpolate", "[1]*x + [0]", domain[standardIndex - nPre - 2][0], domain[standardIndex + nPost + 1][0]);
		} else if ( order == 2 ) {
			func = new TF1("interpolate", "[2]*x^2 + [1]*x + [0]", domain[standardIndex - nPre - 2][0], domain[standardIndex + nPost + 1][0]);
		} else if ( order == 3 ) {
			func = new TF1("interpolate", "[3]*x^3 + [2]*x^2 + [1]*x + [0]", domain[standardIndex - nPre - 2][0], domain[standardIndex + nPost + 1][0]);
		} else if ( order == 4 ) {
			func = new TF1("interpolate", "[4]*x^4 + [3]*x^3 + [2]*x^2 + [1]*x + [0]", domain[standardIndex - nPre - 2][0], domain[standardIndex + nPost + 1][0]);
		} else if ( order == 5 ) {
			func = new TF1("interpolate", "[5]*x^5 + [4]*x^4 + [3]*x^3 + [2]*x^2 + [1]*x + [0]", domain[standardIndex - nPre - 2][0], domain[standardIndex + nPost + 1][0]);
		} else if ( order == 6 ) {
			func = new TF1("interpolate", "[6]*x^6[5]*x^5 + [4]*x^4 + [3]*x^3 + [2]*x^2 + [1]*x + [0]", domain[standardIndex - nPre - 2][0], domain[standardIndex + nPost + 1][0]);
		}

		graph->Fit(func, "RQ");
		static int iTemp = 0;
		TCanvas* can = new TCanvas(Form("canvas_%d", iTemp), "", 1000, 1000);
		graph->SetMarkerStyle(8);
		graph->Draw("AP");
		can->SaveAs(Form("/Users/ychoi/source/ATOM/build/output/entry_%d.png", iTemp));
		iTemp++;
		double returnValue;
		for ( int i = 0; i < order; i++ ) {
			returnValue += (func->GetParameter(i)) * x;
		}

		return returnValue;
	}
}

int main() {
	TEntrySimulation simulation;
	TGraph* graph = new TGraph();
	int iPoint = 0;
	simulation.setSource(2.5);
	simulation.setDetector(30., 15., -3.5);

	std::vector<std::array<double, 2>> ratio;
	for ( double radius = 1.203; radius < 1.205; radius += 0.0001 ) {
		simulation.setCollimator(radius, -0.5, -1.5);
		double count = simulation.doCount();
		graph->SetPoint(iPoint, TMath::Pi() * pow(radius, 2), count / 2);
		ratio.push_back({TMath::Pi() * pow(radius, 2), count / 2});
		break;
		std::cout << radius << "\t" << TMath::Pi() * pow(radius, 2) << "\t" << count / 2 << std::endl;
		iPoint++;
	}

	std::vector<std::array<double, 2>> ratioDifferential;
	differentiate(ratio, ratioDifferential, 3);

	TCanvas* canvas = new TCanvas("can", "", 1100, 1000);
	graph->SetTitle("Sum of Effective Line Ratios (Count by Simul); Collimator Area [mm^2]; Sum of Ratio of Reached/Total Lines at Each Point");
	graph->SetMarkerStyle(8);
	graph->Draw("AP");
	canvas->SetLeftMargin(0.12);
	TF1* func = new TF1("fitFunc", "[0] * x + [1]", 0, 20);
	graph->Fit(func, "R");
	canvas->SaveAs("/Users/ychoi/source/ATOM/build/output/entry.png");

	TCanvas* canvas2 = new TCanvas("can2", "", 1100, 1000);
	TGraph* graph2 = new TGraph();
	for ( int value = 0; value < ratioDifferential.size(); value++ ) {
		std::cout << ratioDifferential[value][0] << "\t" << ratioDifferential[value][1] << std::endl;
		graph2->SetPoint(value, ratioDifferential[value][0], ratioDifferential[value][1]);
	}
	graph2->SetTitle("Derivative");
	graph2->SetMarkerStyle(8);
	graph2->Draw("AP");
	canvas2->SetLeftMargin(0.12);
	canvas2->SaveAs("/Users/ychoi/source/ATOM/build/output/entryDerivative.png");

	TCanvas* canvas3 = new TCanvas("can3", "", 1100, 1000);
	TGraph* graph3 = new TGraph();
	Double_t tilt = func->GetParameter(0);
	Double_t yInAxis = func->GetParameter(1);
	for ( int iRatio = 1; iRatio < ratio.size(); iRatio++ ) {
		graph3->SetPoint(iRatio, ratio[iRatio][0], abs(ratio[iRatio][1] - (ratio[iRatio][0] * tilt) + yInAxis) / ratio[iRatio][1]);
	}
	graph3->SetTitle("Relative Error");
	graph3->SetMarkerStyle(8);
	graph3->Draw("AP");
	canvas3->SetLeftMargin(0.12);
	canvas3->SaveAs("/Users/ychoi/source/ATOM/build/output/relative_error.png");

	delete graph;
	graph = nullptr;

}