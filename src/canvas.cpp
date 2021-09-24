#include "../inc/canvas.h"

Canvas::Canvas(QWidget *parent) : QLabel(parent) {}

void Canvas::mouseDoubleClickEvent(QMouseEvent *event) {
	emit doubleClicked(event->pos(), event->button());
}
