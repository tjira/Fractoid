#pragma once

#include <QDebug>
#include <QMainWindow>
#include <QThread>
#include "ui_preferences.h"
#include "ui_window.h"
#include "canvas.h"
#include "../lib/fractoid/fractoid.h"

namespace Ui {
	class Window;
}

struct Options {
	int iters = 50, bail = 100, w = 1024, h = 576;
	bool smooth = true; int trap = 4, layers = 1, samples = 100000;
	bool fill = true; int fillR = 0, fillG = 0, fillB = 0, solidR = 255, solidG = 255, solidB = 255;
	double cRe = -0.75, cIm = 0, z = 1.3;
	std::vector<double> periodicPar = {0.4, 0.2, 0.3, 0.1, 0.2, 1.1};
	Algorithm alg; Color col;
};

class Window : public QMainWindow {
	Q_OBJECT
public:
	explicit Window(QWidget *parent = nullptr);
	~Window() override;

private:
	template<class F>
	void paint();
	Ui::Window *ui; QThread *worker; Options opt;

public slots:
	void run();
	void preferences();
	void reset();
	void zoom(const QPoint &p, Qt::MouseButton button);
};
