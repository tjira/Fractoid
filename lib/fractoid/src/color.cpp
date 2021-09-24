#include "../fractoid.h"

Color Color::periodic(const std::vector<double> &params) {
	return {
		.outmode = 2,
		.params = params,
	};
}

Color Color::solid(unsigned char r, unsigned char g, unsigned char b) {
	return {
		.outmode = 1,
		.out = {r, g, b},
	};
}

void Color::inside(unsigned char r, unsigned char g, unsigned char b) {
	fill = true;
	in = {r, g, b};
}
