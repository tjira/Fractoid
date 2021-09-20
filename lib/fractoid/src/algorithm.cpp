#include <stdexcept>
#include <random>
#include "../fractoid.h"

Algorithm Algorithm::density(int channels, int samples, int seed) {
	if (channels != 1 && channels != 3) {
		throw std::runtime_error("Invalid number of layers.");
	}
	return Algorithm{
		.alg = 4,
		.layers = channels,
		.samples = samples,
		.seed = seed,
	};
}

Algorithm Algorithm::periodic(bool smooth, int seed) {
	return {
		.alg = 2,
		.smooth = smooth,
		.seed = seed,
		.rnd = Algorithm::randomize(seed),
	};
}

Algorithm Algorithm::orbitrap(int trap, int seed) {
	return Algorithm{
		.alg = 3,
		.trap = trap,
		.seed = seed,
		.rnd = Algorithm::randomize(seed),
	};
}

Algorithm Algorithm::solid(const std::vector<unsigned char> &outside) {
	return Algorithm{
		.alg = 1,
		.out = outside,
	};
}

void Algorithm::color(unsigned char r, unsigned char g, unsigned char b) {
	fill = true;
	in = {r, g, b};
}

std::vector<double> Algorithm::randomize(int seed) {
	std::mt19937 twister(seed); std::uniform_real_distribution<double> dist(0, 1);
	std::vector<double> randomizer(6);
	for (int i = 0; i < 6; i++) {
		randomizer[i] = dist(twister);
	}
	return randomizer;
}
