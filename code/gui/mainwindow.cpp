#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processingstepsettings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create model
    model = new QStringListModel(this);

    // Make data
    QStringList List;
    List << "Example Processing Step 1" << "Example Processing Step 2" << "Example Processing Step 3";

    // Populate our model
    model->setStringList(List);

    // Glue model and view together
    ui->ProcessingSteps->setModel(model);

    // Add additional feature so that
    // we can manually modify the data in ListView
    // It may be triggered by hitting any key or double-click etc.
    ui->ProcessingSteps->
            setEditTriggers(QAbstractItemView::AnyKeyPressed |
                            QAbstractItemView::DoubleClicked);

    modelTable = new ProcessingStepSettings(this);
    ui->tableParams->setModel(modelTable);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    // Add button clicked
    // Adding at the end

    // Get the position
    int row = model->rowCount();

    // Enable add one or more rows
    model->insertRows(row,1);

    // Get the row for Edit mode
    QModelIndex index = model->index(row);

    // Enable item selection and put it edit mode
    ui->ProcessingSteps->setCurrentIndex(index);
    ui->ProcessingSteps->edit(index);
}


void MainWindow::on_deleteButton_clicked()
{
    // Delete button clicked
    // For delete operation,
    // we're dealing with a Model not a View
    // Get the position
    model->removeRows(ui->ProcessingSteps->currentIndex().row(),1);
}

