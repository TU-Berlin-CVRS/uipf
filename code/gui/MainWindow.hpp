#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QListWidgetItem>

#include <QDialog>

#include "ProcessingStepParams.hpp"
#include "ProcessingStepInputs.hpp"

#include "../framework/Configuration.hpp"
#include "../framework/Logger.hpp"

namespace Ui {
class MainWindow;
}

namespace uipf {

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
	void setStepParams(ProcessingStep);

	// sets the table
	void setStepInputs(ProcessingStep);



private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_listProcessingSteps_activated(const QModelIndex & index);
    void on_appendToLog(const Logger::LogType&, const std::string& );

private:
    Ui::MainWindow *ui;

	// all the configuration steps
    QStringListModel *modelStep;

	// all the modules
    QStringListModel *modelModule;

    ProcessingStepParams *modelTableParams;
    ProcessingStepInputs *modelTableInputs;

	// the currently loaded configuration represented in the window
   	Configuration conf_;
};

}; // namespace

#endif // MAINWINDOW_H
