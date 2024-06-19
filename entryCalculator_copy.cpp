#include <iostream>
#include "TMath.h"
#include "TF1.h"
#include "TF2.h"
#include "TH1.h"
#include "TH2.h"
#include "Math/AdaptiveIntegratorMultiDim.h"



void entryCalculator_copy() {
	Double_t s_R; // source의 Active radius
	Double_t c_L; // collimater의 길이
	Double_t c_R; // collimator의 반지름
	Double_t s_A; // source의 Activity
	Double_t R;   // r의 적분구간
	Double_t pi = TMath::Pi();
	Int_t nbinx = 100;

	TH1D* histogram = new TH1D("histo", "histo", nbinx, 0, 2.5);

	for ( Int_t i = 0; i < nbinx + 1; ++i ) {
		//[0] = c_L 
		//[1] = c_R
		// c_R= 0~2.5mm , c_L=10mm 
		s_R = 2.5;  // mm
		c_L = 10.0; // mm
		c_R = i * 2.5 / nbinx;;  // mm
		s_A = 4174; // mm
		if ( s_R > c_R )
			R = c_R;
		else
			R = s_R;

		// cout<<R<<endl;



		//x=alpha or r //y=phi
		TF2* func1 = new TF2("func1", "(1/(4*pi))*2*pi*x*(1-([0]/(sqrt(pow([0],2)+pow(sqrt(pow([1],2)-pow(x*sin(y),2))-x*cos(y),2)))))", 0, R, 0, 2 * pi);
		func1->SetParameter(0, c_L);
		func1->SetParameter(1, c_R);


		// func1->Draw("COL");
		// func1->Draw("Surf");
		// func1->Draw("same");
		//EPS = Epsilon
		//EPS default = 1.e-9

		Double_t integral = func1->Integral(0, R, 0, 2 * pi);
		// cout<< integral <<endl;


		Double_t EffSolAngle = integral * 4 * pi;
		cout << "Radius of collimator = " << c_R << endl;
		cout << "Effective Solid Angle = " << EffSolAngle << endl;
		// Double_t integralError = func1->IntegralError(0,2.5,0,2*pi);
		// cout<< integralError <<endl;
		//불확도는??

		Int_t binNumberFactor = nbinx / 2.5;
		histogram->SetBinContent(c_R * binNumberFactor, EffSolAngle);

	}



	histogram->Draw("");
	TF1* quadratic = new TF1("quadratic", "[0]*pow(x,2)+[1]", 0, 2.5);
	histogram->Fit(quadratic);
	histogram->Draw("SAME");
	// // histogram->Draw("ALP");
	// TF1 *exponential = new TF1("exponential","exp([0]*x+[1])",0,2.5);
	// histogram->Fit(exponential);
	// histogram->Draw("SAME");




	// //면 activity밀도
	// Double_t activityDensity = s_A/(pi*pow(s_R,2));
	// // cout << "ActivityDensity" << endl;
	// // cout << activityDensity << endl;
	// Double_t TheoreticalValue=integral*activityDensity;
	// cout << "Effective Activity" <<endl;
	// cout << TheoreticalValue << endl;

	// //10min Entry
	// Double_t entry_10min = TheoreticalValue*600;
	// cout<< "10min Entry" <<endl;
	// cout<<entry_10min<<endl;


	// // (rho-6)/rho 확인
	// TF2 *func2 = new TF2("func2","2*pi*x*(1-(10/(sqrt(pow(10,2)+pow(sqrt(pow(3,2)-pow(x*sin(y),2))-x*cos(y),2)))))",0,2.5,0,2*pi);
	// cout<<func2->Integral(0,2.5,0,2*pi)<<endl;
	// func2->Draw("Surf");

}