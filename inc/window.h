#pragma once

#include <QDialog>
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include "ui_window.h"
#include "ui_iters.h"
#include "ui_bail.h"
#include "ui_seed.h"
#include "../lib/fractoid/fractoid.h"

namespace Ui {
	class Window;
}

struct Options {
	int iters = 50, bail = 100, w = 1024, h = 576;
	bool smooth = true; int trap = 4, layers = 1, samples = 100000, seed = 34;
	double cRe = -0.75, cIm = 0, z = 1.3;
	Algorithm alg = Algorithm::periodic(true, 34);
};

class Window : public QMainWindow {
	Q_OBJECT
public:
	explicit Window(QWidget *parent = nullptr);
	~Window() override;

private:
	template<class F>
	void paint();
	template<typename T>
	void get();
	Ui::Window *ui; QThread *worker; Options opt;

public slots:
	void run(QAction *act);
};
