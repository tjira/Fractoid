#include "../fractoid.h"
#include "../inc/distance.h"

#define MANOWAR \
zReTemp = zRe; \
zImTemp = zIm; \
zRe = zRe * zRe - zIm * zIm + zRePrev + pRe; \
zIm = zReTemp * (zIm + zIm) + zImPrev + pIm; \
zRePrev = zReTemp; \
zImPrev = zImTemp

Manowar::Manowar(int iters, int bail) : Complex<Manowar>(iters, bail) {}

double Manowar::dist(double pRe, double pIm, double &zMag, int trap) const {
	double zRe = pRe, zIm = pIm, zRePrev = pRe, zImPrev = pIm, zReTemp, zImTemp, val = 100;
	for (int n = 0; n < iters; n++) {
		MANOWAR;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			break;
		} else if (double dist = distance(zRe, zIm, zMag, trap); dist < val) {
			val = dist;
		}
	}
	return val;
}

int Manowar::eta(double pRe, double pIm, double &zMag) const {
	double zRe = pRe, zIm = pIm, zRePrev = pRe, zImPrev = pIm, zReTemp, zImTemp;
	for (int val = 0; val < iters; val++) {
		MANOWAR;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			return val;
		}
	}
	return iters;
}

std::vector<std::vector<double>> Manowar::orbit(double pRe, double pIm, double &zMag) const {
	std::vector<std::vector<double>> orbit;
	double zRe = pRe, zIm = pIm, zRePrev = pRe, zImPrev = pIm, zReTemp, zImTemp;
	for (int n = 0; n < iters; n++) {
		MANOWAR;
		if (zMag = MAG(zRe, zIm), zMag > bail * bail) {
			return orbit;
		}
		orbit.push_back({zRe, zIm});
	}
	return std::vector<std::vector<double>>{};
}
