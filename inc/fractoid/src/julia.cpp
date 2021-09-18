#include "../fractoid.h"
#include "../inc/distance.h"

#define JULIA \
zReTemp = zRe; \
zRe = zRe * zRe - zIm * zIm + aRe; \
zIm = zReTemp * (zIm + zIm) + aIm

Julia::Julia(int iters, int bail) : Complex<Julia>(iters, bail) {}

double Julia::dist(double pRe, double pIm, double &zMag, int trap) const {
	double zRe = pRe, zIm = pIm, zReTemp, val = 100;
	for (int n = 0; n < iters; n++) {
		JULIA;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			break;
		} else if (double dist = distance(zRe, zIm, zMag, trap); dist < val) {
			val = dist;
		}
	}
	return val;
}

int Julia::eta(double pRe, double pIm, double &zMag) const {
	double zRe = pRe, zIm = pIm, zReTemp;
	for (int val = 0; val < iters; val++) {
		JULIA;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			return val;
		}
	}
	return iters;
}

std::vector<std::vector<double>> Julia::orbit(double pRe, double pIm, double &zMag) const {
	std::vector<std::vector<double>> orbit;
	double zRe = pRe, zIm = pIm, zReTemp;
	for (int n = 0; n < iters; n++) {
		JULIA;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			return orbit;
		}
		orbit.push_back({zRe, zIm});
	}
	return std::vector<std::vector<double>>{};
}

void Julia::params(double aReIn, double aImIn) {
	aRe = aReIn;
	aIm = aImIn;
}
