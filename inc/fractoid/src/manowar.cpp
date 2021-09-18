#include "../fractoid.h"
#include "../inc/distance.h"

#define MANOWAR \
zReTemp = zRe; \
zImTemp = zIm; \
zRe = zRe * zRe - zIm * zIm + zRePrev + pRe; \
zIm = zReTemp * (zIm + zIm) + zImPrev + pIm; \
zRePrev = zReTemp; \
zImPrev = zImTemp

Manowar::Manowar(int iters, int bailout) : Complex<Manowar>(iters, bailout) {}

double Manowar::dist(double pRe, double pIm, double &zMag, int trap) const {
	double zRe = pRe, zIm = pIm, zRePrev = pRe, zImPrev = pIm, zReTemp, zImTemp, value = 100;
	for (int n = 0; n < iters; n++) {
		MANOWAR;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			break;
		} else if (double dist = distance(zRe, zIm, zMag, trap); dist < value) {
			value = dist;
		}
	}
	return value;
}

int Manowar::eta(double pRe, double pIm, double &zMag) const {
	double zRe = pRe, zIm = pIm, zRePrev = pRe, zImPrev = pIm, zReTemp, zImTemp;
	for (int value = 0; value < iters; value++) {
		MANOWAR;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			return value;
		}
	}
	return iters;
}

std::vector<std::vector<double>> Manowar::orbit(double pRe, double pIm, double &zMag) const {
	std::vector<std::vector<double>> orbit;
	double zRe = pRe, zIm = pIm, zRePrev = pRe, zImPrev = pIm, zReTemp, zImTemp;
	for (int n = 0; n < iters; n++) {
		MANOWAR;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			return orbit;
		}
		orbit.push_back({zRe, zIm});
	}
	return std::vector<std::vector<double>>{};
}
