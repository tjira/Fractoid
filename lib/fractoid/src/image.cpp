#include "../../lodepng/lodepng.h"
#include "../fractoid.h"

Image::Image(int w, int h) : w(w), h(h), ratio((double) w / h), size(4 * w * h), canvas(size, 255) {}

unsigned int& Image::operator()(int i, int j, int ch) {
	return canvas[4 * w * i + 4 * j + ch];
}

void Image::brightness(double val) {
	for (int i = 0; i < canvas.size(); i += 4) {
		canvas[i + 0] = (unsigned char) MIN(val * canvas[i + 0], 255);
		canvas[i + 1] = (unsigned char) MIN(val * canvas[i + 1], 255);
		canvas[i + 2] = (unsigned char) MIN(val * canvas[i + 2], 255);
	}
}

std::vector<unsigned char> Image::raw() const {
	return std::vector<unsigned char>(canvas.begin(), canvas.end());
}

void Image::fill(unsigned char r, unsigned char g, unsigned char b) {
	for (int i = 0; i < canvas.size(); i += 4) {
		canvas[i + 0] = r;
		canvas[i + 1] = g;
		canvas[i + 2] = b;
	}
}

void Image::normalize() {
	for (int ch = 0; ch < 3; ch++) {
		double max = 0;
		for (int i = 0; i < size; i += 4) {
			max = canvas[i + ch] > max ? canvas[i + ch] : max;
		}
		for (int i = 0; i < size; i += 4) {
			canvas[i + ch] = (unsigned int) ((double) canvas[i + ch] / max * 255.0);
		}
	}
}

unsigned int Image::save(const std::string &filename) {
	return lodepng::encode(filename, std::vector<unsigned char>(canvas.begin(), canvas.end()), w, h);
}

void Image::set(int i, int j, unsigned char r, unsigned char g, unsigned char b) {
	canvas[4 * w * i + 4 * j + 0] = r;
	canvas[4 * w * i + 4 * j + 1] = g;
	canvas[4 * w * i + 4 * j + 2] = b;
}
