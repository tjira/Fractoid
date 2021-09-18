#include "../fractoid.h"
#include "../inc/distance.h"

#define PHOENIX \
zReTemp = zRe; \
zImTemp = zIm; \
zRe = zRe * zRe - zIm * zIm + 0.56667 - 0.5 * zRePrev; \
zIm = zReTemp * (zIm + zIm) - 0.5 * zImPrev; \
zRePrev = zReTemp; \
zImPrev = zImTemp

Phoenix::Phoenix(int iters, int bail) : Complex<Phoenix>(iters, bail) {}

double Phoenix::dist(double pRe, double pIm, double &zMag, int trap) const {
	double zRe = -pIm, zIm = pRe, zRePrev = aRe, zImPrev = aIm, zReTemp, zImTemp, val = 100;
	for (int n = 0; n < iters; n++) {
		PHOENIX;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			break;
		} else if (double dist = distance(zRe, zIm, zMag, trap); dist < val) {
			val = dist;
		}
	}
	return val;
}

int Phoenix::eta(double pRe, double pIm, double &zMag) const {
	double zRe = -pIm, zIm = pRe, zRePrev = aRe, zImPrev = aIm, zReTemp, zImTemp;
	for (int val = 0; val < iters; val++) {
		PHOENIX;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			return val;
		}
	}
	return iters;
}

std::vector<std::vector<double>> Phoenix::orbit(double pRe, double pIm, double &zMag) const {
	std::vector<std::vector<double>> orbit;
	double zRe = -pIm, zIm = pRe, zRePrev = aRe, zImPrev = aIm, zReTemp, zImTemp;
	for (int n = 0; n < iters; n++) {
		PHOENIX;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			return orbit;
		}
		orbit.push_back({zRe, zIm});
	}
	return std::vector<std::vector<double>>{};
}

void Phoenix::params(double aReIn, double aImIn) {
	aRe = aReIn;
	aIm = aImIn;
}
