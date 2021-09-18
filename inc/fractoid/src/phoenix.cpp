#include "../fractoid.h"
#include "../inc/distance.h"

#define PHOENIX \
zReTemp = zRe; \
zImTemp = zIm; \
zRe = zRe * zRe - zIm * zIm + 0.56667 - 0.5 * zRePrev; \
zIm = zReTemp * (zIm + zIm) - 0.5 * zImPrev; \
zRePrev = zReTemp; \
zImPrev = zImTemp

Phoenix::Phoenix(int iters, int bailout) : Complex<Phoenix>(iters, bailout) {}

double Phoenix::dist(double pRe, double pIm, double &zMag, int trap) const {
	double zRe = -pIm, zIm = pRe, zRePrev = CRe, zImPrev = CIm, zReTemp, zImTemp, value = 100;
	for (int n = 0; n < iters; n++) {
		PHOENIX;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			break;
		} else if (double dist = distance(zRe, zIm, zMag, trap); dist < value) {
			value = dist;
		}
	}
	return value;
}

int Phoenix::eta(double pRe, double pIm, double &zMag) const {
	double zRe = -pIm, zIm = pRe, zRePrev = CRe, zImPrev = CIm, zReTemp, zImTemp;
	for (int value = 0; value < iters; value++) {
		PHOENIX;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			return value;
		}
	}
	return iters;
}

std::vector<std::vector<double>> Phoenix::orbit(double pRe, double pIm, double &zMag) const {
	std::vector<std::vector<double>> orbit;
	double zRe = -pIm, zIm = pRe, zRePrev = CRe, zImPrev = CIm, zReTemp, zImTemp;
	for (int n = 0; n < iters; n++) {
		PHOENIX;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			return orbit;
		}
		orbit.push_back({zRe, zIm});
	}
	return std::vector<std::vector<double>>{};
}

void Phoenix::params(double CReIn, double CImIn) {
	CRe = CReIn;
	CIm = CImIn;
}
