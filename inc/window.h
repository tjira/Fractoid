#pragma once

#include <QDialog>
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include "ui_window.h"
#include "ui_preferences.h"
#include "../lib/fractoid/fractoid.h"

namespace Ui {
	class Window;
}

struct Options {
	int iters = 50, bail = 100, w = 1024, h = 576;
	bool smooth = true; int trap = 4, layers = 1, samples = 100000, seed = 34;
	bool fill = true; int fillR = 0, fillG = 0, fillB = 0, solidR = 255, solidG = 255, solidB = 255;
	double cRe = -0.75, cIm = 0, z = 1.3;
	Algorithm alg;
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
	void run(QAction *act);
	void preferences();
};
