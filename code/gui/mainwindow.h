#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>

#include <QDialog>

#include "processingstepsettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	// sets a processing step list
	void setStepList(QStringList);
	
	// sets a Module name list
	void setModuleList(QStringList);

private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::MainWindow *ui;

	// all the configuration steps
    QStringListModel *model;
    
	// all the modules
    QStringListModel *modelModule;

    ProcessingStepSettings *modelTable;

};

#endif // MAINWINDOW_H
