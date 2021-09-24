#include <chrono>
#include "../lib/argparse/argparse.hpp"
#include "../lib/fractoid/fractoid.h"

template<typename F>
void execute(const argparse::ArgumentParser& program) {
	Algorithm alg = Algorithm::eta(program.get<bool>("--smooth"));
	Color col = Color::periodic({0.4, 0.2, 0.3, 0.1, 0.2, 1.1});
	if (int(program.is_used("--orbitrap")) + int(program.is_used("--density")) > 1) {
		throw std::runtime_error("Can't use two algorithms at the same time.");
	} else if (program.is_used("--orbitrap")) {
		alg = Algorithm::orbitrap(program.get<int>("--orbitrap"));
	} else if (program.is_used("--density")) {
		alg = Algorithm::density(program.get<int>("--density"), program.get<int>("--samples"));
	}
	if (program.is_used("--solid")) {
		auto color = program.get<std::vector<unsigned char>>("--solid");
		col = Color::solid(color[0], color[1], color[2]);
	}
	if (program.is_used("--fill")) {
		auto color = program.get<std::vector<unsigned char>>("--fill");
		col.inside(color[0], color[1], color[2]);
	}
	F fractal(
		program.get<int>("--iters"),
		program.get<int>("--bailout")
	);
	if (typeid(F) == typeid(Julia)) {
		auto params = program.get<std::vector<double>>("--julia");
		fractal.params(params[0], params[1]);
	}
	if (typeid(F) == typeid(Phoenix)) {
		auto params = program.get<std::vector<double>>("--phoenix");
		fractal.params(params[0], params[1]);
	}
	auto start = std::chrono::high_resolution_clock::now();
	Image image = fractal.paint(
		program.get<std::vector<double>>("--location")[0],
		program.get<std::vector<double>>("--location")[1],
		program.get<double>("--zoom"), alg, col,
		program.get<std::vector<int>>("--resolution")[0],
		program.get<std::vector<int>>("--resolution")[1]
	);
	auto end = std::chrono::high_resolution_clock::now();
	if (program.is_used("--brightness")) image.brightness(program.get<double>("--brightness"));
	[[maybe_unused]] unsigned int error = image.save(program.get<std::string>("--output"));
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
}

int main(int argc, char *argv[]) {
	argparse::ArgumentParser program("./fractoid-cli");
	program.add_description("Command line interface for Fractoid library.");
	program.add_argument("name").help("name of the fractal to paint");
	program.add_argument("-b", "--bailout").help("bail radius").default_value(50).action([](const std::string &value) { return std::stoi(value); });
	program.add_argument("-f", "--fill").help("fill color").action([](const std::string& value) { return (unsigned char) std::stoi(value); }).nargs(3);
	program.add_argument("-i", "--iters").help("maximum number of iterations before bail").default_value(100).action([](const std::string &value) { return std::stoi(value); });
	program.add_argument("-l", "--location").help("real and imaginary part of location location").default_value(std::vector<double>{0.0, 0.0}).action([](const std::string& value) { return std::stod(value); }).nargs(2);
	program.add_argument("-o", "--output").help("output filename").default_value(std::string("fractal.png"));
	program.add_argument("-r", "--resolution").help("resolution of the image").default_value(std::vector<int>{1920, 1080}).action([](const std::string &value) { return std::stoi(value); }).nargs(2);
	program.add_argument("-z", "--zoom").help("fractal zoom").default_value(1.0).action([](const std::string &value) { return std::stod(value); });
	program.add_argument("--smooth").help("smoothing").default_value(false).implicit_value(true);
	program.add_argument("--solid").help("solid alg").action([](const std::string& value) { return (unsigned char) std::stoi(value); }).nargs(3);
	program.add_argument("--orbitrap").help("orbitrap alg").action([](const std::string &value) { return std::stoi(value); });
	program.add_argument("--density").help("density alg").action([](const std::string &value) { return std::stoi(value); });
	program.add_argument("--samples").help("number of samples for density alg").default_value(100000).action([](const std::string &value) { return std::stoi(value); });
	program.add_argument("--julia").help("additional Julia set complex parameter").default_value(std::vector<double>{0, 1}).action([](const std::string &value) { return std::stod(value); }).nargs(2);
	program.add_argument("--phoenix").help("additional Phoenix set complex parameter").default_value(std::vector<double>{0, 0}).action([](const std::string &value) { return std::stod(value); }).nargs(2);
	program.add_argument("--brightness").help("brightness").action([](const std::string &value) { return std::stod(value); });
	try {
		program.parse_args(argc, argv);
	}
	catch (const std::runtime_error &err) {
		std::cout << err.what() << std::endl << program;
		return 0;
	}
	if (program.get<std::string>("name") == "burningship") execute<BurningShip>(program);
	else if (program.get<std::string>("name") == "julia") execute<Julia>(program);
	else if (program.get<std::string>("name") == "mandelbrot") execute<Mandelbrot>(program);
	else if (program.get<std::string>("name") == "manowar") execute<Manowar>(program);
	else if (program.get<std::string>("name") == "phoenix") execute<Phoenix>(program);
	else throw std::runtime_error("Fractal unknown.");
	return 0;
}
