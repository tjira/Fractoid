#include "../inc/window.h"
#include "ui_window.h"

#include <QDebug>

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window), worker(new QThread(this)) {
	ui->setupUi(this); setFixedSize(opt.w, opt.h + ui->menu->sizeHint().height());
	connect(ui->quit, &QAction::triggered, this, &QApplication::quit);
	connect(ui->formulas, &QActionGroup::triggered, this, &Window::run);
	connect(ui->algs, &QActionGroup::triggered, this, &Window::run);
	ui->formulas->triggered(ui->mandelbrot);
}

Window::~Window() {
	delete ui; delete worker;
}
template<class F>
void Window::paint() {
	F fractal(opt.iters, opt.bail);
	worker = QThread::create([this, fractal] {
		std::vector<unsigned char> img = fractal.paint(opt.cRe, opt.cIm, opt.z, opt.alg, opt.w, opt.h).raw();
		ui->image->setPixmap(QPixmap::fromImage(QImage(img.data(), opt.w, opt.h, QImage::Format_RGBA8888)));
	});
	worker->start();
}

void Window::run(QAction *act) {
	if (act->text() == "Burning Ship") {
		opt.cRe = -0.45, opt.cIm = 0.55, opt.z = 1.05;
		paint<BurningShip>();
	} else if (act->text() == "Julia") {
		opt.cRe = 0, opt.cIm = 0, opt.z = 1.1;
		paint<Julia>();
	} else if (act->text() == "Mandelbrot") {
		opt.cRe = -0.75, opt.cIm = 0, opt.z = 1.2;
		paint<Mandelbrot>();
	} else if (act->text() == "Manowar") {
		opt.cRe = -0.5, opt.cIm = 0, opt.z = 1.3;
		paint<Manowar>();
	} else if (act->text() == "Phoenix") {
		opt.cRe = 0, opt.cIm = 0, opt.z = 1.4;
		paint<Phoenix>();
	} else if (act->text() == "Solid") {
		opt.alg = Algorithm::solid({255, 255, 255});
		run(ui->formulas->checkedAction());
	} else if (act->text() == "Periodic") {
		opt.alg = Algorithm::periodic(true, 34);
		run(ui->formulas->checkedAction());
	} else if (act->text() == "Orbitrap") {
		opt.alg = Algorithm::orbitrap(4, 34);
		run(ui->formulas->checkedAction());
	} else if (act->text() == "Density") {
		opt.alg = Algorithm::density(1, 1000000, 34);
		run(ui->formulas->checkedAction());
	}
	opt.alg.color(0, 0, 0);
}
