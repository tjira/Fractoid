#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include "../lib/fractoid/fractoid.h"

namespace Ui {
	class Window;
}

struct Options {
	int iters = 50, bail = 100, w = 1024, h = 576;
	double cRe = -0.75, cIm = 0, z = 1.3;
};

class Window : public QMainWindow {
Q_OBJECT
public:
	explicit Window(QWidget *parent = nullptr);
	~Window() override;

private:
	void display(const Image &img);
	template<class F>
	void paint();
	Ui::Window *ui; QThread *worker; Options opt;

public slots:
	void run(QAction *act);
};
