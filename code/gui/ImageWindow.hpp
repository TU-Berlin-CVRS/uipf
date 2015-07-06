#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QGraphicsView>

#include <../framework/ModuleManager.hpp>

namespace uipf {

class ImageWindow : public QGraphicsView
{
    Q_OBJECT

public:
    ImageWindow(ModuleManager& mm, QWidget *parent = 0) : QGraphicsView(parent), mm_(mm) {};
    ImageWindow(ModuleManager& mm, QGraphicsScene * scene, QWidget * parent = 0) : QGraphicsView(scene, parent), mm_(mm) {};
    ~ImageWindow() {};

protected:
	void closeEvent(QCloseEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:

	ModuleManager& mm_;

};

} // namespace

#endif // MAINWINDOW_H
