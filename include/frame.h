#ifndef __FRAME
#define __FRAME__

#include <iostream>
#include <array>

#include "TROOT.h"

class Frame {
private:
    std::array<std::array<Double_t,4>,4> frame;
public:
    Frame();
    Frame(Double_t num1, Double_t num2, Double_t num3, Double_t num4, Double_t num5, Double_t num6, Double_t num7, Double_t num8, Double_t num9, Double_t num10, Double_t num11, Double_t num12, Double_t num13, Double_t num14, Double_t num15, Double_t num16);
    Frame(Double_t numArr[]);
    Frame(Double_t numArr1[], Double_t numArr2[], Double_t numArr3[], Double_t numArr4[]);
    Frame(std::array<std::array<Double_t,4>,4> doubleArr);
    Frame(const Frame& obj);
    
    friend std::ostream& operator<<(std::ostream& os, const Frame& cpyFrame);
    friend Frame operator+(Frame preFrame, Frame postFrame);
    friend Frame operator+(Int_t num, Frame postFrame);
    friend Frame operator+(Frame preFrame, Int_t num);
    friend Frame operator-(Frame preFrame, Frame postFrame);
    friend Frame operator-(Int_t num, Frame postFrame);
    friend Frame operator-(Frame preFrame, Int_t num);
    friend Frame operator*(Frame preFrame, Frame postFrame);
    friend Frame operator*(Int_t num, Frame postFrame);
    friend Frame operator*(Frame preFrame, Int_t num);
    friend Frame operator/(Frame preFrame, Frame postFrame);
    friend Frame operator/(Int_t num, Frame postFrame);
    friend Frame operator/(Frame preFrame, Int_t num);

    Double_t GetPixelValue(Int_t x, Int_t y) const;
};

class AnalysisTool {
private:
    std::array<Double_t,4> arr;
public:

};


#endif