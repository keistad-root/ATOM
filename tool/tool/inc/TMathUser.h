#ifndef __TMATHUSER__
#define __TMATHUSER__

#include<array>
#include<iostream>
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

	return {effectiveValue, effectiveError};
}

#endif