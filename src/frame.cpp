#include "frame.h"

Frame::Frame() {
    frame = {{0,},};
}

Frame::Frame(Double_t num1, Double_t num2, Double_t num3, Double_t num4, Double_t num5, Double_t num6, Double_t num7, Double_t num8, Double_t num9, Double_t num10, Double_t num11, Double_t num12, Double_t num13, Double_t num14, Double_t num15, Double_t num16) {
    frame = {num1, num2, num3, num4, num5, num6, num7, num8, num9, num10, num11, num12, num13, num14, num15, num16};
}

Frame::Frame(Double_t numArr[]) {
    frame[0] = {numArr[0],numArr[1],numArr[2],numArr[3]};
    frame[1] = {numArr[4],numArr[5],numArr[6],numArr[7]};
    frame[2] = {numArr[8],numArr[9],numArr[10],numArr[11]};
    frame[3] = {numArr[12],numArr[13],numArr[14],numArr[15]};
}

Frame::Frame(Double_t numArr1[], Double_t numArr2[], Double_t numArr3[], Double_t numArr4[]) {
    frame[0] = std::array<Double_t,4>{*numArr1,*numArr1+1,*numArr1+2,*numArr1+3};
    frame[1] = std::array<Double_t,4>{*numArr2,*numArr2+1,*numArr2+2,*numArr2+3};
    frame[2] = std::array<Double_t,4>{*numArr3,*numArr3+1,*numArr3+2,*numArr3+3};
    frame[3] = std::array<Double_t,4>{*numArr4,*numArr4+1,*numArr4+2,*numArr4+3};
}

Frame::Frame(std::array<std::array<Double_t,4>,4> doubleArr) {
    frame = doubleArr;
}

Frame::Frame(const Frame& obj) {
    frame = obj.frame;
}

std::ostream& operator<<(std::ostream& os, const Frame& cpyFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            os << cpyFrame.frame[j][i] << " ";
        }
        os << std::endl;
    }
    return os;
}

Frame operator+(Frame preFrame, Frame postFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            postFrame.frame[j][i] += preFrame.frame[j][i];
        }
    }
    return postFrame;
}

Frame operator+(Int_t num, Frame postFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            postFrame.frame[j][i] += num;
        }
    }
    return postFrame;
}

Frame operator+(Frame preFrame, Int_t num) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            preFrame.frame[j][i] += num;
        }
    }
    return preFrame;
}

Frame operator-(Frame preFrame, Frame postFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            postFrame.frame[j][i] -= preFrame.frame[j][i];
        }
    }
    return postFrame;
}

Frame operator-(Int_t num, Frame postFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            postFrame.frame[j][i] = num - postFrame.frame[j][i];
        }
    }
    return postFrame;
}

Frame operator-(Frame preFrame, Int_t num) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            preFrame.frame[j][i] -= num;
        }
    }
    return preFrame;
}

Frame operator*(Frame preFrame, Frame postFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            postFrame.frame[j][i] *= preFrame.frame[j][i];
        }
    }
    return postFrame;
}

Frame operator*(Int_t num, Frame postFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            postFrame.frame[j][i] *= num;
        }
    }
    return postFrame;
}

Frame operator*(Frame preFrame, Int_t num) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            preFrame.frame[j][i] *= num;
        }
    }
    return preFrame;
}

Frame operator/(Frame preFrame, Frame postFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            postFrame.frame[j][i] /= preFrame.frame[j][i];
        }
    }
    return postFrame;
}

Frame operator/(Int_t num, Frame postFrame) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            postFrame.frame[j][i] = num / postFrame.frame[j][i];
        }
    }
    return postFrame;
}

Frame operator/(Frame preFrame, Int_t num) {
    for (int8_t i = 0; i < 4; i++) {
        for (int8_t j = 0; j < 4; j++) {
            preFrame.frame[j][i] /= num;
        }
    }
    return preFrame;
}

Double_t Frame::GetPixelValue(Int_t x, Int_t y) const {
    return frame[x][y];
}