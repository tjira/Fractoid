#include <stdexcept>
#include <random>
#include "../fractoid.h"

Algorithm Algorithm::density(int channels, int samples, int seed) {
	if (channels != 1 && channels != 3) {
		throw std::runtime_error("Invalid number of channels.");
	}
	return Algorithm{
		.algorithm = 4,
		.channels = channels,
		.samples = samples,
		.seed = seed,
	};
}

Algorithm Algorithm::periodic(bool smooth, int seed) {
	return {
		.algorithm = 2,
		.smooth = smooth,
		.seed = seed,
		.randomizer = Algorithm::randomize(seed),
	};
}

Algorithm Algorithm::orbitrap(int trap, int seed) {
	return Algorithm{
		.algorithm = 3,
		.trap = trap,
		.seed = seed,
		.randomizer = Algorithm::randomize(seed),
	};
}

Algorithm Algorithm::solid(const std::vector<unsigned char> &outside) {
	return Algorithm{
		.algorithm = 1,
		.outside = outside,
	};
}

void Algorithm::color(const std::vector<unsigned char> &insideIn) {
	fill = true;
	inside = insideIn;
}

std::vector<double> Algorithm::randomize(int seed) {
	std::mt19937 twister(seed); std::uniform_real_distribution<double> dist(0, 1);
	std::vector<double> randomizer(6);
	for (int i = 0; i < 6; i++) {
		randomizer[i] = dist(twister);
	}
	return randomizer;
}
