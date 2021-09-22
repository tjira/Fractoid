#include "../inc/window.h"

Window::Window(QWidget *parent) : QMainWindow(parent), ui(new Ui::Window), worker(new QThread(this)) {
	ui->setupUi(this); setFixedSize(opt.w, opt.h + ui->menu->sizeHint().height());
	connect(ui->algs, &QActionGroup::triggered, this, &Window::run);
	connect(ui->formulas, &QActionGroup::triggered, this, [this]() { run(ui->algs->checkedAction()); });
	connect(ui->preferences, &QAction::triggered, this, &Window::preferences);
	connect(ui->quit, &QAction::triggered, this, &QApplication::quit);
	ui->formulas->triggered(ui->algs->checkedAction());
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
		opt.alg = Algorithm::solid(opt.solidR, opt.solidG, opt.solidB);
		if (opt.fill) opt.alg.color(opt.fillR, opt.fillG, opt.fillB);
		run(ui->formulas->checkedAction());
	} else if (act->text() == "Periodic") {
		opt.alg = Algorithm::periodic(opt.smooth, opt.seed);
		if (opt.fill) opt.alg.color(opt.fillR, opt.fillG, opt.fillB);
		run(ui->formulas->checkedAction());
	} else if (act->text() == "Orbitrap") {
		opt.alg = Algorithm::orbitrap(opt.trap, opt.seed);
		if (opt.fill) opt.alg.color(opt.fillR, opt.fillG, opt.fillB);
		run(ui->formulas->checkedAction());
	} else if (act->text() == "Density") {
		opt.alg = Algorithm::density(opt.layers, opt.samples, opt.seed);
		run(ui->formulas->checkedAction());
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
	dui.samples->setValue(opt.samples);
	dui.seed->setValue(opt.seed);
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
		opt.samples = dui.samples->value();
		opt.seed = dui.seed->value();
		opt.smooth = dui.smooth->isChecked();
		opt.solidR = dui.solidR->value();
		opt.solidG = dui.solidG->value();
		opt.solidB = dui.solidB->value();
		opt.trap = dui.trap->currentIndex() + 1;
		run(ui->algs->checkedAction());
	}
	delete dialog;
}
