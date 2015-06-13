#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processingstepsettings.h"

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

}

MainWindow::~MainWindow()
{
    delete ui;
    delete modelStep;
    delete modelModule;
    delete modelTable;
}

// sets a processing step list
void MainWindow::setStepList(QStringList list){
	modelStep->setStringList(list);
}

// sets a Module list
void MainWindow::setModuleList(QStringList list){
	modelModule->setStringList(list);
}

// sets the table
void MainWindow::setStepParams(uipf::ProcessingStep proStep){
	modelTable->setProcessingStep(proStep);	
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

