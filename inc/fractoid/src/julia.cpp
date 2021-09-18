#include "../fractoid.h"
#include "../inc/distance.h"

#define JULIA \
zReTemp = zRe; \
zRe = zRe * zRe - zIm * zIm + CRe; \
zIm = zReTemp * (zIm + zIm) + CIm

Julia::Julia(int iters, int bailout) : Complex<Julia>(iters, bailout) {}

double Julia::dist(double pRe, double pIm, double &zMag, int trap) const {
	double zRe = pRe, zIm = pIm, zReTemp, value = 100;
	for (int n = 0; n < iters; n++) {
		JULIA;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			break;
		} else if (double dist = distance(zRe, zIm, zMag, trap); dist < value) {
			value = dist;
		}
	}
	return value;
}

int Julia::eta(double pRe, double pIm, double &zMag) const {
	double zRe = pRe, zIm = pIm, zReTemp;
	for (int value = 0; value < iters; value++) {
		JULIA;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			return value;
		}
	}
	return iters;
}

std::vector<std::vector<double>> Julia::orbit(double pRe, double pIm, double &zMag) const {
	std::vector<std::vector<double>> orbit;
	double zRe = pRe, zIm = pIm, zReTemp;
	for (int n = 0; n < iters; n++) {
		JULIA;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			return orbit;
		}
		orbit.push_back({zRe, zIm});
	}
	return std::vector<std::vector<double>>{};
}

void Julia::params(double CReIn, double CImIn) {
	CRe = CReIn;
	CIm = CImIn;
}
