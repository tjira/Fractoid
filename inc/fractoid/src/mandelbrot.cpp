#include "../fractoid.h"
#include "../inc/distance.h"

#define MANDELBROT \
zReTemp = zRe; \
zRe = zRe * zRe - zIm * zIm + pRe; \
zIm = zReTemp * (zIm + zIm) + pIm

Mandelbrot::Mandelbrot(int iters, int bailout) : Complex<Mandelbrot>(iters, bailout) {}

double Mandelbrot::dist(double pRe, double pIm, double &zMag, int trap) const {
	double zRe = 0, zIm = 0, zReTemp, value = 100;
	for (int n = 0; n < iters; n++) {
		MANDELBROT;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			break;
		} else if (double dist = distance(zRe, zIm, zMag, trap); dist < value) {
			value = dist;
		}
	}
	return value;
}

int Mandelbrot::eta(double pRe, double pIm, double &zMag) const {
	double zRe = 0, zIm = 0, zReTemp;
	for (int value = 0; value < iters; value++) {
		MANDELBROT;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			return value;
		}
	}
	return iters;
}

std::vector<std::vector<double>> Mandelbrot::orbit(double pRe, double pIm, double &zMag) const {
	std::vector<std::vector<double>> orbit;
	double zRe = 0, zIm = 0, zReTemp;
	for (int n = 0; n < iters; n++) {
		MANDELBROT;
		if (zMag = MAG(zRe, zIm), zMag > bailout * bailout) {
			return orbit;
		}
		orbit.push_back({zRe, zIm});
	}
	return std::vector<std::vector<double>>{};
}
