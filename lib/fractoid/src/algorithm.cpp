#include <stdexcept>
#include "../fractoid.h"

Algorithm Algorithm::density(int channels, int samples) {
	if (channels != 1 && channels != 3) {
		throw std::runtime_error("Invalid number of layers.");
	}
	return Algorithm{
		.alg = 3,
		.layers = channels,
		.samples = samples,
	};
}

Algorithm Algorithm::eta(bool smooth) {
	return {
		.alg = 1,
		.smooth = smooth,
	};
}

Algorithm Algorithm::orbitrap(int trap) {
	return Algorithm{
		.alg = 2,
		.trap = trap,
	};
}
