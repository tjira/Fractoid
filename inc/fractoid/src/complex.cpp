#include <algorithm>
#include <random>
#include "../fractoid.h"

template<class F>
Complex<F>::Complex(int iters, int bailout) {
	this->iters = iters;
	this->bailout = bailout;
}

template<class F>
F Complex<F>::copy(int bailoutIn, int itersIn) const {
	return F(itersIn, bailoutIn);
}

template<class F>
void Complex<F>::density(Image &image, double centerRe, double centerIm, double zoom, const Algorithm &alg) const {
	int width = image.resolution()[0], height = image.resolution()[1];
	double reMin = centerRe - 1.5 * width / height, reMax = centerRe + 1.5 * width / height;
	double imMin = -centerIm - 1.5, imMax = -centerIm + 1.5;
	std::mt19937 twister(alg.seed);
	std::uniform_real_distribution<double> real(reMin, reMax);
	std::uniform_real_distribution<double> imag(imMin, imMax);
	for (int index = 0; index < alg.channels; index++) {
		F fractal = copy(bailout, iters * (alg.channels == 1 ? 1 : pow(10, 2 - index)));
		std::vector<unsigned int> channel(4 * width * height);
		#pragma omp parallel for
		for (int sample = 0; sample < alg.samples; sample++) {
			double zMag; std::vector<std::vector<double>> points = fractal.orbit(real(twister), imag(twister), zMag);
			for (auto &point : points) {
				if (point[0] < reMin || point[0] > reMax || point[1] < imMin || point[1] > imMax) continue;
				int i = int(((point[1] + centerIm) * height * zoom + 1.5 * height) / 3.0);
				int j = int(((point[0] - centerRe) * height * zoom + 1.5 * width) / 3.0);
				channel[4 * width * i + 4 * j + index]++;
				if (alg.channels == 1) {
					channel[4 * width * i + 4 * j + 1]++;
					channel[4 * width * i + 4 * j + 2]++;
				}
			}
		}
		double max = *std::max_element(channel.begin(), channel.end());
		if (max != 255) {
			for(unsigned int &value : channel) {
				value = (unsigned int) (value / max * 255);
			}
		}
		image.add(std::vector<unsigned char>(channel.begin(), channel.end()));
	}
}

template<class F>
void Complex<F>::normal(Image &image, double centerRe, double centerIm, double zoom, const Algorithm &alg) const {
	int width = image.resolution()[0], height = image.resolution()[1]; double scale = 1.0 / zoom / height;
	#pragma omp parallel for default(none) shared(image, centerRe, centerIm, zoom, alg, width, height, scale)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double pRe = centerRe - (1.5 * width - 3 * j) * scale;
			double pIm = -centerIm - (1.5 * height - 3 * i) * scale;
			double zMag; double value = alg.algorithm == 3 ? dist(pRe, pIm, zMag, alg.trap) : eta(pRe, pIm, zMag);
			unsigned char r = alg.inside[0], g = alg.inside[1], b = alg.inside[2];
			if (zMag > bailout * bailout || !alg.fill) {
				if (alg.smooth && zMag > bailout * bailout) {
					value += 1.44269504f * log(2.0 * log(bailout) / log(zMag));
				}
				switch (alg.algorithm) {
					case 1: solid:
						r = alg.outside[0];
						g = alg.outside[1];
						b = alg.outside[2];
						break;
					case 2: trig:
						r = (unsigned char) ((sin(alg.randomizer[0] * value + alg.randomizer[1]) + 1) * 127.5);
						g = (unsigned char) ((sin(alg.randomizer[2] * value + alg.randomizer[3]) + 1) * 127.5);
						b = (unsigned char) ((sin(alg.randomizer[3] * value + alg.randomizer[5]) + 1) * 127.5);
						break;
					case 3:
						value = log(ABS((value)));
						goto trig;
					default:
						goto solid;
				}
			}
			image.set(i, j, r, g, b);
		}
	}
}

template<class F>
Image Complex<F>::paint(double centerRe, double centerIm, double zoom, const Algorithm &alg, int width, int height) const {
	Image image(width, height);
	if (alg.algorithm == 1 || alg.algorithm == 2 || alg.algorithm == 3) {
		normal(image, centerRe, centerIm, zoom, alg);
	}
	else if (alg.algorithm == 4) {
		image.fill(0, 0, 0);
		density(image, centerRe, centerIm, zoom, alg);
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
