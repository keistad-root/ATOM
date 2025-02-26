#ifndef __TMATHUSER__
#define __TMATHUSER__

#include<array>
#include<iostream>
#include<vector>
#include<math.h>

inline double derivative(const std::array<double, 3> domain, const std::array<double, 3> range) {
	double h1 = domain[2] - domain[1];
	double h2 = domain[1] - domain[0];

	double d1 = (range[2] - range[1]) / h1;
	double d2 = (range[1] - range[0]) / h2;

	return (d1 + d2) / 2.;
}

inline std::array<double, 2> getEffectiveNumber(double value, double error) {
	int digit = floor(log10(error));
	if ( floor(error * pow(10, -digit)) == 1 ) {
		digit--;
	}
	double effectiveValue = floor(value * pow(10, -digit)) / pow(10, -digit);
	double effectiveError = floor(error * pow(10, -digit)) / pow(10, -digit);

	if ( digit < -100 ) {
		effectiveValue = 0;
		effectiveError = 0;
	}
	return {effectiveValue, effectiveError};
}

inline std::vector<double> createGaussianKernel(int kernelSize, double sigma) {
	std::vector<double> kernel(kernelSize);
	double sum = 0.0;
	int center = kernelSize / 2;
	for ( int i = 0; i < kernelSize; ++i ) {
		kernel[i] = std::exp(-0.5 * std::pow((i - center) / sigma, 2)) / (sigma * std::sqrt(2 * M_PI));
		sum += kernel[i];
	}
	// Normalize kernel
	for ( double& value : kernel ) {
		value /= sum;
	}
	return kernel;
}

// 가우시안 스무딩
inline std::vector<double> gaussianSmoothing(const std::vector<double>& data, int kernelSize, double sigma) {
	std::vector<double> kernel = createGaussianKernel(kernelSize, sigma);
	std::vector<double> smoothedData(data.size());
	int halfKernel = kernelSize / 2;

	for ( size_t i = 0; i < data.size(); ++i ) {
		double smoothedValue = 0.0;
		for ( int j = -halfKernel; j <= halfKernel; ++j ) {
			int index = std::min(std::max(int(i) + j, 0), int(data.size()) - 1);
			smoothedValue += data[index] * kernel[j + halfKernel];
		}
		smoothedData[i] = smoothedValue;
	}
	return smoothedData;
}

inline std::vector<double> localMaximum(const std::vector<double>& data) {
	std::vector<double> maximums;
	for ( int i = 1; i < data.size() - 1; i++ ) {
		if ( data[i - 1] > data[i] && data[i + 1] > data[i] ) {
			maximums.push_back(data[i]);
		}
	}
}

#endif