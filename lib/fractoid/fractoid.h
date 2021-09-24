#pragma once

#include <string>
#include <vector>

#define ABS(A) ((A) < 0 ? -(A) : (A))
#define MAG(A, B) ((A) * (A) + (B) * (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

struct Algorithm {
	static Algorithm density(int channels, int samples);
	static Algorithm eta(bool smooth);
	static Algorithm orbitrap(int trap);
	int alg; bool smooth; int trap, layers, samples;
};

struct Color {
	static Color periodic(const std::vector<double> &params);
	static Color solid(unsigned char r, unsigned char g, unsigned char b);
	void inside(unsigned char r, unsigned char g, unsigned char b);
	bool fill; int outmode; std::vector<unsigned char> in = {0, 0, 0}, out; std::vector<double> params;
};

class Image {
	template<class>
	friend class Complex;
public:
	Image(int w, int h);
	unsigned int& operator()(int i, int j, int ch);
	void brightness(double val);
	[[nodiscard]] std::vector<unsigned char> raw() const;
	void fill(unsigned char r, unsigned char g, unsigned char b);
	void normalize();
	[[nodiscard]] unsigned int save(const std::string &filename);
	void set(int i, int j, unsigned char r, unsigned char g, unsigned char b);

private:
	int w, h, size; double ratio; std::vector<unsigned int> canvas;
};

template<class F>
class Complex {
public:
	[[nodiscard]] F copy(int itersIn, int bailoutIn) const;
	[[nodiscard]] Image paint(double cRe, double cIm, double z, const Algorithm &alg, const Color &col, int w = 1920, int h = 1080) const;
	virtual void params(double CReIn, double CImIn);

protected:
	Complex(int iters, int bailout);
	int bail = 5, iters = 100;

private:
	void density(Image &img, double cRe, double cIm, double z, const Algorithm &alg) const;
	void normal(Image &img, double cRe, double cIm, double z, const Algorithm &alg, const Color &col) const;
	virtual double dist(double pRe, double pIm, double &zMag, int trap) const;
	virtual int eta(double pRe, double pIm, double &zMag) const;
	[[nodiscard]] virtual std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const;
};

class BurningShip : public Complex<BurningShip> {
	friend class Complex;
public:
	BurningShip(int iters, int bail);

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
};

class Julia : public Complex<Julia> {
	friend class Complex;
public:
	Julia(int iters, int bail);
	void params(double aReIn, double aImIn) override;

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
	double aRe = 0, aIm = 1;
};

class Mandelbrot : public Complex<Mandelbrot> {
	friend class Complex;
public:
	Mandelbrot(int iters, int bail);

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
};

class Manowar : public Complex<Manowar> {
	friend class Complex;
public:
	Manowar(int iters, int bail);

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
};

class Phoenix : public Complex<Phoenix> {
	friend class Complex;
public:
	Phoenix(int iters, int bail);
	void params(double aReIn, double aImIn) override;

private:
	double dist(double pRe, double pIm, double &zMag, int trap) const override;
	int eta(double pRe, double pIm, double &zMag) const override;
	[[nodiscard]] std::vector<std::vector<double>> orbit(double pRe, double pIm, double &zMag) const override;
	double aRe = 0, aIm = 0;
};
