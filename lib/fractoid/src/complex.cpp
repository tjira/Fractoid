#include <algorithm>
#include <chrono>
#include <random>
#include "../fractoid.h"

template<class F>
Complex<F>::Complex(int iters, int bailout) {
	this->iters = iters;
	this->bail = bailout;
}

template<class F>
F Complex<F>::copy(int itersIn, int bailoutIn) const {
	return F(itersIn, bailoutIn);
}

template<class F>
void Complex<F>::density(Image &img, double cRe, double cIm, double z, const Algorithm &alg) const {
	std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> rRe(cRe - 1.5 * img.ratio, cRe + 1.5 * img.ratio);
	std::uniform_real_distribution<double> rIm(-cIm - 1.5, -cIm + 1.5);
	for (int layer = 0; layer < alg.layers; layer++) {
		F fractal = copy(iters * pow(10, layer), bail);
		#pragma omp parallel for default(none) shared(img, cRe, cIm, z, alg, mt, rRe, rIm, fractal, layer)
		for (int sample = 0; sample < alg.samples; sample++) {
			double zMag; std::vector<std::vector<double>> ps = fractal.orbit(rRe(mt), rIm(mt), zMag);
			for (auto &p : ps) {
				int i = int(((p[1] + cIm) * img.h * z + 1.5 * img.h) / 3.0);
				int j = int(((p[0] - cRe) * img.h * z + 1.5 * img.w) / 3.0);
				if (i < 0 || j < 0 || i >= img.h || j >= img.w) continue;
				img(i, j, layer)++;
				if (alg.layers == 1) {
					img(i, j, 1)++;
					img(i, j, 2)++;
				}
			}
		}
		img.normalize();
	}
}

template<class F>
void Complex<F>::normal(Image &img, double cRe, double cIm, double z, const Algorithm &alg, const Color &col) const {
	double scale = 1.0 / z / img.h;
	#pragma omp parallel for default(none) shared(img, cRe, cIm, z, alg, col, scale)
	for (int i = 0; i < img.h; i++) {
		for (int j = 0; j < img.w; j++) {
			double pRe = cRe - (1.5 * img.w - 3 * j) * scale;
			double pIm = -cIm - (1.5 * img.h - 3 * i) * scale;
			double zMag; double val = alg.alg == 2 ? dist(pRe, pIm, zMag, alg.trap) : eta(pRe, pIm, zMag);
			unsigned char r = col.in[0], g = col.in[1], b = col.in[2];
			if (zMag > bail * bail || !col.fill) {
				switch (col.outmode) {
					case 1: solid:
						r = col.out[0];
						g = col.out[1];
						b = col.out[2];
						break;
					case 2:
						if (alg.alg == 1) {if (alg.smooth && zMag > bail * bail) {
							val += 1.44269504f * log(2.0 * log(bail) / log(zMag));
						}} else if (alg.alg == 2) {
							val = log(ABS((val)));
						}
						r = (unsigned char) ((sin(col.params[0] * val + col.params[1]) + 1) * 127.5);
						g = (unsigned char) ((sin(col.params[2] * val + col.params[3]) + 1) * 127.5);
						b = (unsigned char) ((sin(col.params[4] * val + col.params[5]) + 1) * 127.5);
						break;
					default:
						goto solid;
				}
			}
			img.set(i, j, r, g, b);
		}
	}
}

template<class F>
Image Complex<F>::paint(double cRe, double cIm, double z, const Algorithm &alg, const Color &col, int w, int h) const {
	Image image(w, h);
	if (alg.alg == 1 || alg.alg == 2) {
		normal(image, cRe, cIm, z, alg, col);
	}
	else if (alg.alg == 3) {
		image.fill(0, 0, 0);
		density(image, cRe, cIm, z, alg);
	}
	return image;
}

template<class F>
double Complex<F>::dist(double pRe, double pIm, double &zMag, int trap) const {
	return 0;
}

template<class F>
int Complex<F>::eta(double pRe, double pIm, double &zMag) const {
	return 0;
}

template<class F>
std::vector<std::vector<double>> Complex<F>::orbit(double pRe, double pIm, double &zMag) const {
	return {};
}

template<class F>
void Complex<F>::params(double CReIn, double CImIn) {}


template Complex<BurningShip>::Complex(int, int);
template Complex<Julia>::Complex(int, int);
template Complex<Mandelbrot>::Complex(int, int);
template Complex<Manowar>::Complex(int, int);
template Complex<Phoenix>::Complex(int, int);

template Image Complex<BurningShip>::paint(double, double, double, Algorithm const&, Color const&, int, int) const;
template Image Complex<Julia>::paint(double, double, double, Algorithm const&, Color const&, int, int) const;
template Image Complex<Mandelbrot>::paint(double, double, double, Algorithm const&, Color const&, int, int) const;
template Image Complex<Manowar>::paint(double, double, double, Algorithm const&, Color const&, int, int) const;
template Image Complex<Phoenix>::paint(double, double, double, Algorithm const&, Color const&, int, int) const;
