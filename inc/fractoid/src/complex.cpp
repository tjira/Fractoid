#include <algorithm>
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
	std::mt19937 twister(alg.seed);
	std::uniform_real_distribution<double> real(cRe - 1.5 * img.ratio, cRe + 1.5 * img.ratio);
	std::uniform_real_distribution<double> imag(-cIm - 1.5, -cIm + 1.5);
	for (int index = 0; index < alg.layers; index++) {
		F fractal = copy(iters * (alg.layers == 1 ? 1 : pow(10, 2 - index)), bail);
		std::vector<unsigned int> layer(4 * img.w * img.h);
		#pragma omp parallel for default(none) shared(img, cRe, cIm, z, alg, twister, real, imag, fractal, index, layer)
		for (int sample = 0; sample < alg.samples; sample++) {
			double zMag; std::vector<std::vector<double>> ps = fractal.orbit(real(twister), imag(twister), zMag);
			for (auto &p : ps) {
				if (p[0] < real.a() || p[0] > real.b() || p[1] < imag.a() || p[1] > imag.b()) continue;
				int i = int(((p[1] + cIm) * img.h * z + 1.5 * img.h) / 3.0);
				int j = int(((p[0] - cRe) * img.h * z + 1.5 * img.w) / 3.0);
				layer[4 * img.w * i + 4 * j + index]++;
				if (alg.layers == 1) {
					layer[4 * img.w * i + 4 * j + 1]++;
					layer[4 * img.w * i + 4 * j + 2]++;
				}
			}
		}
		double max = *std::max_element(layer.begin(), layer.end());
		if (max != 255) {
			for(unsigned int &val : layer) {
				val = (unsigned int) (val / max * 255);
			}
		}
		img.add(std::vector<unsigned char>(layer.begin(), layer.end()));
	}
}

template<class F>
void Complex<F>::normal(Image &img, double cRe, double cIm, double z, const Algorithm &alg) const {
	double scale = 1.0 / z / img.h;
	#pragma omp parallel for default(none) shared(img, cRe, cIm, z, alg, scale)
	for (int i = 0; i < img.h; i++) {
		for (int j = 0; j < img.w; j++) {
			double pRe = cRe - (1.5 * img.w - 3 * j) * scale;
			double pIm = -cIm - (1.5 * img.h - 3 * i) * scale;
			double zMag; double val = alg.alg == 3 ? dist(pRe, pIm, zMag, alg.trap) : eta(pRe, pIm, zMag);
			unsigned char r = alg.in[0], g = alg.in[1], b = alg.in[2];
			if (zMag > bail * bail || !alg.fill) {
				switch (alg.alg) {
					case 1: solid:
						r = alg.out[0];
						g = alg.out[1];
						b = alg.out[2];
						break;
					case 2: trig:
						if (alg.smooth && zMag > bail * bail) {
							val += 1.44269504f * log(2.0 * log(bail) / log(zMag));
						}
						r = (unsigned char) ((sin(alg.rnd[0] * val + alg.rnd[1]) + 1) * 127.5);
						g = (unsigned char) ((sin(alg.rnd[2] * val + alg.rnd[3]) + 1) * 127.5);
						b = (unsigned char) ((sin(alg.rnd[3] * val + alg.rnd[5]) + 1) * 127.5);
						break;
					case 3:
						val = log(ABS((val)));
						goto trig;
					default:
						goto solid;
				}
			}
			img.set(i, j, r, g, b);
		}
	}
}

template<class F>
Image Complex<F>::paint(double cRe, double cIm, double z, const Algorithm &alg, int w, int h) const {
	Image image(w, h);
	if (alg.alg == 1 || alg.alg == 2 || alg.alg == 3) {
		normal(image, cRe, cIm, z, alg);
	}
	else if (alg.alg == 4) {
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

template Image Complex<BurningShip>::paint(double, double, double, Algorithm const&, int, int) const;
template Image Complex<Julia>::paint(double, double, double, Algorithm const&, int, int) const;
template Image Complex<Mandelbrot>::paint(double, double, double, Algorithm const&, int, int) const;
template Image Complex<Manowar>::paint(double, double, double, Algorithm const&, int, int) const;
template Image Complex<Phoenix>::paint(double, double, double, Algorithm const&, int, int) const;
