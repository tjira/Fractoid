#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QPoint>

class Canvas : public QLabel {
	Q_OBJECT
public:
	explicit Canvas(QWidget *parent = nullptr);

private:
	void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
	void doubleClicked(const QPoint &point, Qt::MouseButton button);
};
