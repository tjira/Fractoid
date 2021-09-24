#include "../inc/window.h"

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window), worker(new QThread(this)) {
	ui->setupUi(this); setFixedSize(opt.w, opt.h + ui->menu->sizeHint().height());
	connect(ui->algs, &QActionGroup::triggered, this, &Window::run);
	connect(ui->cols, &QActionGroup::triggered, this, &Window::run);
	connect(ui->formulas, &QActionGroup::triggered, this, &Window::run);
	connect(ui->preferences, &QAction::triggered, this, &Window::preferences);
	connect(ui->quit, &QAction::triggered, this, &QApplication::quit);
	ui->formulas->triggered(ui->cols->checkedAction());
}

Window::~Window() {
	delete ui; delete worker;
}

template<class F>
void Window::paint() {
	F fractal(opt.iters, opt.bail);
	worker = QThread::create([this, fractal] {
		std::vector<unsigned char> img = fractal.paint(opt.cRe, opt.cIm, opt.z, opt.alg, opt.col, opt.w, opt.h).raw();
		ui->image->setPixmap(QPixmap::fromImage(QImage(img.data(), opt.w, opt.h, QImage::Format_RGBA8888)));
	});
	worker->start();
}

void Window::run() {
	if (ui->algs->checkedAction()->text() == "ETA") {
		opt.alg = Algorithm::eta(opt.smooth);
	} else if (ui->algs->checkedAction()->text() == "Orbitrap") {
		opt.alg = Algorithm::orbitrap(opt.trap);
	} else if (ui->algs->checkedAction()->text() == "Density") {
		opt.alg = Algorithm::density(opt.layers, opt.samples);
	}
	if (ui->cols->checkedAction()->text() == "Periodic") {
		opt.col = Color::periodic(opt.periodicPar);
	} else if (ui->cols->checkedAction()->text() == "Solid") {
		opt.col = Color::solid(opt.solidR, opt.solidG, opt.solidB);
	}
	if (opt.fill) opt.col.inside(opt.fillR, opt.fillG, opt.fillB);
	if (ui->formulas->checkedAction()->text() == "Burning Ship") {
		opt.cRe = -0.45, opt.cIm = 0.55, opt.z = 1.05;
		paint<BurningShip>();
	} else if (ui->formulas->checkedAction()->text() == "Julia") {
		opt.cRe = 0, opt.cIm = 0, opt.z = 1.1;
		paint<Julia>();
	} else if (ui->formulas->checkedAction()->text() == "Mandelbrot") {
		opt.cRe = -0.75, opt.cIm = 0, opt.z = 1.2;
		paint<Mandelbrot>();
	} else if (ui->formulas->checkedAction()->text() == "Manowar") {
		opt.cRe = -0.5, opt.cIm = 0, opt.z = 1.3;
		paint<Manowar>();
	} else if (ui->formulas->checkedAction()->text() == "Phoenix") {
		opt.cRe = 0, opt.cIm = 0, opt.z = 1.4;
		paint<Phoenix>();
	}
}

void Window::preferences() {
	auto *dialog = new QDialog();
	Ui_Preferences dui{}; dui.setupUi(dialog);
	dui.bail->setValue(opt.bail);
	dui.fill->setChecked(opt.fill);
	dui.fillR->setValue(opt.fillR);
	dui.fillG->setValue(opt.fillG);
	dui.fillB->setValue(opt.fillB);
	dui.iters->setValue(opt.iters);
	dui.layers->setCurrentIndex(opt.layers == 1 ? 0 : 1);
	dui.periodicPar1->setValue(opt.periodicPar[0]);
	dui.periodicPar2->setValue(opt.periodicPar[1]);
	dui.periodicPar3->setValue(opt.periodicPar[2]);
	dui.periodicPar4->setValue(opt.periodicPar[3]);
	dui.periodicPar5->setValue(opt.periodicPar[4]);
	dui.periodicPar6->setValue(opt.periodicPar[5]);
	dui.samples->setValue(opt.samples);
	dui.smooth->setChecked(opt.smooth);
	dui.solidR->setValue(opt.solidR);
	dui.solidG->setValue(opt.solidG);
	dui.solidB->setValue(opt.solidB);
	dui.trap->setCurrentIndex(opt.trap - 1);
	if (dialog->exec() == QDialog::Accepted) {
		opt.bail = dui.bail->value();
		opt.fill = dui.fill->isChecked();
		opt.fillR = dui.fillR->value();
		opt.fillG = dui.fillG->value();
		opt.fillB = dui.fillB->value();
		opt.iters = dui.iters->value();
		opt.layers = dui.layers->currentIndex() ? 3 : 1;
		opt.periodicPar[0] = dui.periodicPar1->value();
		opt.periodicPar[1] = dui.periodicPar2->value();
		opt.periodicPar[2] = dui.periodicPar3->value();
		opt.periodicPar[3] = dui.periodicPar4->value();
		opt.periodicPar[4] = dui.periodicPar5->value();
		opt.periodicPar[5] = dui.periodicPar6->value();
		opt.samples = dui.samples->value();
		opt.smooth = dui.smooth->isChecked();
		opt.solidR = dui.solidR->value();
		opt.solidG = dui.solidG->value();
		opt.solidB = dui.solidB->value();
		opt.trap = dui.trap->currentIndex() + 1;
		run();
	}
	delete dialog;
}
