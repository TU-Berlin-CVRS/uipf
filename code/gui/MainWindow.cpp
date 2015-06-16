#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <iostream>

using namespace std;
using namespace uipf;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    // Create model
    modelStep = new QStringListModel(this);
    modelModule = new QStringListModel(this);
    modelTableParams = new ProcessingStepParams(this);
    modelTableInputs = new ProcessingStepInputs(this);

    // Glue model and view together
    ui->listProcessingSteps->setModel(modelStep);
    ui->tableParams->setModel(modelTableParams);
    ui->tableInputs->setModel(modelTableInputs);
	ui->comboModule->setModel(modelModule);

    // Add additional feature so that
    // we can manually modify the data in ListView
    // It may be triggered by hitting any key or double-click etc.
    ui->listProcessingSteps->
            setEditTriggers(QAbstractItemView::AnyKeyPressed |
                            QAbstractItemView::DoubleClicked);


	// react to changes in the ListView
	// TODO improve this to react on any selection change: http://stackoverflow.com/questions/2468514/how-to-get-the-selectionchange-event-in-qt
    connect(ui->listProcessingSteps, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(on_listProcessingSteps_activated(const QModelIndex &)));

    connect(Logger::instance(), SIGNAL (logEvent(const Logger::LogType&,const std::string&)), this, SLOT (on_appendToLog(const Logger::LogType&,const std::string&)));
}

void MainWindow::on_appendToLog(const Logger::LogType& eType,const std::string& strText)
{
	// For colored Messages we need html :-/
	QString strColor = (eType == Logger::WARNING ? "Blue" : eType == Logger::ERROR ? "Red" : "Green");
	QString alertHtml = "<font color=\""+strColor+"\">" + QString(strText.c_str()) + "</font>";
	ui->tbLog->appendHtml(alertHtml);
	ui->tbLog->verticalScrollBar()->setValue(ui->tbLog->verticalScrollBar()->maximum());

}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelStep;
    delete modelModule;
    delete modelTableParams;
    delete modelTableInputs;
}

// TODO this should later be called from the File->Load Data Flow menu
void MainWindow::loadDataFlow(string filename) {


	conf_.load(filename);

    // only for debug, print the loaded config
	conf_.print();

	// set the names of the processing steps:
	QStringList list;
	map<string, ProcessingStep> chain = conf_.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		list << it->first.c_str();
	}
	modelStep->setStringList(list);
}


// sets a Module list
void MainWindow::setModuleList(QStringList list){
	modelModule->setStringList(list);
}


void MainWindow::on_addButton_clicked() {
    // Add button clicked
    // Adding at the end

    // Get the position of the selected item
    int row = modelStep->rowCount();

    // Enable add one or more rows
    modelStep->insertRows(row,1);

    // Get the row for Edit mode
    QModelIndex index = modelStep->index(row);

    // Enable item selection and put it edit mode
    ui->listProcessingSteps->setCurrentIndex(index);
    ui->listProcessingSteps->edit(index);
}


void MainWindow::on_deleteButton_clicked() {
    // Delete button clicked
    // For delete operation,
    // we're dealing with a Model not a View
    // Get the position
    modelStep->removeRows(ui->listProcessingSteps->currentIndex().row(),1);
}

// gets called when a processing step is selected
void MainWindow::on_listProcessingSteps_activated(const QModelIndex & index) {

	map<string, ProcessingStep> chain = conf_.getProcessingChain();

	string selectedStep = ui->listProcessingSteps->model()->data(ui->listProcessingSteps->currentIndex()).toString().toStdString();
	cout << "selected " << selectedStep << endl;
	ProcessingStep proStep = chain[selectedStep];

	modelTableParams->setProcessingStep(proStep);
	modelTableInputs->setProcessingStep(proStep);
}

