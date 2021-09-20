#include "../fractoid.h"
#include "../inc/distance.h"

#define BURNINGSHIP \
zReTemp = zRe; \
zRe = zRe * zRe - zIm * zIm + pRe; \
zIm = ABS(zReTemp * (zIm + zIm)) + pIm

BurningShip::BurningShip(int iters, int bail) : Complex<BurningShip>(iters, bail) {}

double BurningShip::dist(double pRe, double pIm, double &zMag, int trap) const {
	double zRe = 0, zIm = 0, zReTemp, val = 100;
	for (int n = 0; n < iters; n++) {
		BURNINGSHIP;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			break;
		} else if (double dist = distance(zRe, zIm, zMag, trap); dist < val) {
			val = dist;
		}
	}
	return val;
}

int BurningShip::eta(double pRe, double pIm, double &zMag) const {
	double zRe = 0, zIm = 0, zReTemp;
	for (int val = 0; val < iters; val++) {
		BURNINGSHIP;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			return val;
		}
	}
	return iters;
}

std::vector<std::vector<double>> BurningShip::orbit(double pRe, double pIm, double &zMag) const {
	std::vector<std::vector<double>> orbit;
	double zRe = 0, zIm = 0, zReTemp;
	for (int n = 0; n < iters; n++) {
		BURNINGSHIP;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			return orbit;
		}
		orbit.push_back({zRe, zIm});
	}
	return std::vector<std::vector<double>>{};
}
