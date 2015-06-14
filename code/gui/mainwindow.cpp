#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processingstepsettings.h"

#include <iostream>

using namespace std;
using namespace uipf;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    // Create model
    modelStep = new QStringListModel(this);
    modelModule = new QStringListModel(this);
    modelTable = new ProcessingStepSettings(this);

    // Glue model and view together
    ui->ProcessingSteps->setModel(modelStep);
    ui->tableParams->setModel(modelTable);
	ui->comboBox->setModel(modelModule);

    // Add additional feature so that
    // we can manually modify the data in ListView
    // It may be triggered by hitting any key or double-click etc.
    ui->ProcessingSteps->
            setEditTriggers(QAbstractItemView::AnyKeyPressed |
                            QAbstractItemView::DoubleClicked);


	// react to changes in the ListView
	// TODO improve this to react on any selection change: http://stackoverflow.com/questions/2468514/how-to-get-the-selectionchange-event-in-qt
    connect(ui->ProcessingSteps, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(on_ProcessingSteps_activated(const QModelIndex &)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelStep;
    delete modelModule;
    delete modelTable;
}

// TODO this should later be called from the File->Load Data Flow menu
void MainWindow::loadDataFlow(std::string filename) {


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
    ui->ProcessingSteps->setCurrentIndex(index);
    ui->ProcessingSteps->edit(index);
}


void MainWindow::on_deleteButton_clicked() {
    // Delete button clicked
    // For delete operation,
    // we're dealing with a Model not a View
    // Get the position
    modelStep->removeRows(ui->ProcessingSteps->currentIndex().row(),1);
}

// gets called when a processing step is selected
void MainWindow::on_ProcessingSteps_activated(const QModelIndex & index) {

	map<string, ProcessingStep> chain = conf_.getProcessingChain();

	string selectedStep = ui->ProcessingSteps->model()->data(ui->ProcessingSteps->currentIndex()).toString().toStdString();
	std::cout << "selected " << selectedStep << std::endl;
	ProcessingStep proStep = chain[selectedStep];

	modelTable->setProcessingStep(proStep);


}
