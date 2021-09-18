#include "../lib/lodepng/lodepng.h"
#include "../fractoid.h"

Image::Image(int width, int height) : canvas(4 * width * height, 255) {
	this->width = width;
	this->height = height;
}

void Image::add(std::vector<unsigned char> canvasIn) {
	for (int i = 0; i < canvas.size(); i++) {
		canvas[i] += canvasIn[i];
	}
}

void Image::brightness(double value) {
	for (int i = 0; i < canvas.size(); i += 4) {
		canvas[i + 0] = (unsigned char) MIN(value * canvas[i + 0], 255);
		canvas[i + 1] = (unsigned char) MIN(value * canvas[i + 1], 255);
		canvas[i + 2] = (unsigned char) MIN(value * canvas[i + 2], 255);
	}
}

void Image::fill(unsigned char r, unsigned char g, unsigned char b) {
	for (int i = 0; i < canvas.size(); i += 4) {
		canvas[i + 0] = r;
		canvas[i + 1] = g;
		canvas[i + 2] = b;
	}
}

std::vector<int> Image::resolution() const {
	return {width, height};
}

unsigned int Image::save(const std::string &filename) const {
	return lodepng::encode(filename, canvas, width, height);
}

void Image::set(int i, int j, unsigned char r, unsigned char g, unsigned char b) {
	canvas[4 * width * i + 4 * j + 0] = r;
	canvas[4 * width * i + 4 * j + 1] = g;
	canvas[4 * width * i + 4 * j + 2] = b;
}
