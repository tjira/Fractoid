#pragma once

#include <string>
#include <vector>

#define ABS(A) ((A) < 0 ? -(A) : (A))
#define MAG(A, B) ((A) * (A) + (B) * (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

struct Algorithm {
	static Algorithm density(int channels, int samples, int seed = 1);
	static Algorithm periodic(bool smooth, int seed = 1);
	static Algorithm orbitrap(int trap, int seed = 1);
	static Algorithm solid(const std::vector<unsigned char> &outside);
	void color(const std::vector<unsigned char> &insideIn);
	int algorithm; bool fill, smooth; int trap, channels, samples, seed;
	std::vector<unsigned char> inside = {0, 0, 0}, outside; std::vector<double> randomizer;

private:
	static std::vector<double> randomize(int seed);
};

class Image {
public:
	Image(int width, int height);
	[[nodiscard]] unsigned int save(const std::string &filename) const;
	void add(std::vector<unsigned char> canvasIn);
	[[nodiscard]] std::vector<int> resolution() const;
	void set(int i, int j, unsigned char r, unsigned char g, unsigned char b);
	void fill(unsigned char r, unsigned char g, unsigned char b);
	void brightness(double value);

private:
	int width, height; std::vector<unsigned char> canvas;
};

template<class F>
class Complex {
public:
	[[nodiscard]] F copy(int bailoutIn, int itersIn) const;
	[[nodiscard]] Image paint(double centerRe, double centerIm, double zoom, const Algorithm &col, int width = 1920, int height = 1080) const;
	virtual void params(double CReIn, double CImIn);

protected:
	Complex(int iters, int bailout);
	int bailout = 5, iters = 100;

private:
	void density(Image &image, double centerRe, double centerIm, double zoom, const Algorithm &col) const;
	void normal(Image &image, double centerRe, double centerIm, double zoom, const Algorithm &col) const;
	virtual double dist(double pRe, double pIm, double &zMag, int trap) const;
	virtual int eta(double pRe, double pIm, double &zMag) const;
	[[nodiscard]] virtual std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const;
};

class BurningShip : public Complex<BurningShip> {
	friend class Complex;
public:
	BurningShip(int iters, int bailout);

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
};

class Julia : public Complex<Julia> {
	friend class Complex;
public:
	Julia(int iters, int bailout);
	void params(double CReIn, double CImIn) override;

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
	double CRe = 0, CIm = 1;
};

class Mandelbrot : public Complex<Mandelbrot> {
	friend class Complex;
public:
	Mandelbrot(int iters, int bailout);

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
};

class Manowar : public Complex<Manowar> {
	friend class Complex;
public:
	Manowar(int iters, int bailout);

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
};

class Phoenix : public Complex<Phoenix> {
	friend class Complex;
public:
	Phoenix(int iters, int bailout);
	void params(double CReIn, double CImIn) override;

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
	double CRe = 0, CIm = 0;
};
