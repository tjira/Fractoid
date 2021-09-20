#include "../inc/window.h"
#include "ui_window.h"

#include <QDebug>

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window), worker(new QThread(this)) {
	ui->setupUi(this); setFixedSize(opt.w, opt.h + ui->menu->sizeHint().height());
	run(ui->mandelbrot);
	connect(ui->quit, &QAction::triggered, this, &QApplication::quit);
	connect(ui->formulas, &QActionGroup::triggered, this, &Window::run);
}

Window::~Window() {
	delete ui; delete worker;
}

void Window::display(const Image &img) {
	std::vector<unsigned char> raw = img.raw();
	ui->image->setPixmap(QPixmap::fromImage(QImage(raw.data(), opt.w, opt.h, QImage::Format_RGBA8888)));
}

template<class F>
void Window::paint() {
	F fractal(opt.iters, opt.bail);
	Algorithm alg = Algorithm::periodic(true, 34);
	alg.color(0, 0, 0);
	worker = QThread::create([this, fractal, alg] {
		Image img = fractal.paint(opt.cRe, opt.cIm, opt.z, alg, opt.w, opt.h);
		display(img);
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
	}
}
