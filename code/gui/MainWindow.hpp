#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QListWidgetItem>

#include <QDialog>

#include "ProcessingStepParams.hpp"

#include "../framework/Configuration.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void loadDataFlow(std::string);

	// sets a Module name list
	void setModuleList(QStringList);

	// sets the table
	void setStepParams(uipf::ProcessingStep);

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_ProcessingSteps_activated(const QModelIndex & index);

private:
    Ui::MainWindow *ui;

	// all the configuration steps
    QStringListModel *modelStep;

	// all the modules
    QStringListModel *modelModule;

    ProcessingStepParams *modelTable;

	// the currently loaded configuration represented in the window
   	uipf::Configuration conf_;
};

#endif // MAINWINDOW_H
