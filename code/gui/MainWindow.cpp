#include <QScrollBar>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardItemModel>

#include <iostream>

#include "../framework/ModuleManager.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "ComboBoxSourceStep.hpp"
#include "ComboBoxSourceOutput.hpp"

using namespace std;
using namespace uipf;

// constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // Create models
    modelStep = new QStringListModel(this);
    modelModule = new QStringListModel(this);
    modelTableParams = new ProcessingStepParams(this);
    modelTableInputs = new ProcessingStepInputs(this);
	model = new QStandardItemModel(this); // TODO merge with input

    // Glue model and view together
    ui->listProcessingSteps->setModel(modelStep);
    ui->tableParams->setModel(modelTableParams);
    ui->tableInputs->setModel(modelTableInputs);
	ui->comboModule->setModel(modelModule);
	ui->tableView->setModel(model); // TODO merge with input

	// Processing Step Names: allow to manually modify the data
    // -> It may be triggered by hitting any key or double-click etc.
    ui->listProcessingSteps-> setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);


	// set up slots for signals

	// react to selection of the entries
	// -> TODO improve this to react on any selection change: http://stackoverflow.com/questions/2468514/how-to-get-the-selectionchange-event-in-qt
    connect(ui->listProcessingSteps, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(on_listProcessingSteps_activated(const QModelIndex &)));
    // react to changes in the entries
    connect(modelStep, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
            this, SLOT(stepNameChanged()));
	// logger
    connect(Logger::instance(), SIGNAL (logEvent(const Logger::LogType&,const std::string&)),
			this, SLOT (on_appendToLog(const Logger::LogType&,const std::string&)));


    // commands for menu bar
    createActions();
    createMenus();

    // window settings
    setMinimumSize(800, 600);
    resize(1200, 800);


    // set initial state
	currentFileName = "newFile";
    setWindowTitle(tr((currentFileName + string(" - ") + WINDOW_TITLE).c_str()));


	// set the possible modules
	setModuleList();
}

// destructor
MainWindow::~MainWindow() {
    delete ui;
    delete modelStep;
    delete modelModule;
    delete modelTableParams;
    delete modelTableInputs;
    delete model;

    deleteActions();
}


// sets a Module list
/*
list	list of all availanle modules
*/
void MainWindow::setModuleList() {

// TODO refactor
	QStringList list_modules;
	map<string, MetaData> modules = mm_.getAllModuleMetaData();
	for (auto it = modules.begin(); it!=modules.end(); ++it) {
		list_modules << it->first.c_str();
	}

	modelModule->setStringList(list_modules);
}

// loads a new configuration from file
void MainWindow::loadDataFlow(string filename)
{
	currentFileName = filename;
    setWindowTitle(tr((currentFileName + string(" - ") + WINDOW_TITLE).c_str()));

    // configuration changed
	beforeConfigChange(); // TODO maybe better reset undo stack for new files
	conf_.load(currentFileName);

    // only for debug, print the loaded config
	conf_.print();

	// save is now activated
	saveAct->setEnabled(true);

	// set the names of the processing steps:
	QStringList list;
	map<string, ProcessingStep> chain = conf_.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		list << it->first.c_str();
	}
	modelStep->setStringList(list);

	// TODO reset other models
}


// From here: SLOTS -------------------------------------------------------------------------------------------------------------------------------

// TODO: comment!
void MainWindow::on_appendToLog(const Logger::LogType& eType,const std::string& strText) {
	// For colored Messages we need html :-/
	QString strColor = (eType == Logger::WARNING ? "Blue" : eType == Logger::ERROR ? "Red" : "Green");
	QString alertHtml = "<font color=\""+strColor+"\">" + QString(strText.c_str()) + "</font>";
	ui->tbLog->appendHtml(alertHtml);
	ui->tbLog->verticalScrollBar()->setValue(ui->tbLog->verticalScrollBar()->maximum());
}


// Add button clicked - allows to add a new Processing Step
// Step is always added at the end of the list
void MainWindow::on_addButton_clicked() {
    // Get the position of the selected item
    int row = modelStep->rowCount();

    // Enable add one or more rows
    modelStep->insertRows(row,1);

    // Get the row for Edit mode
    QModelIndex index = modelStep->index(row);

    // Enable item selection and put it edit mode
    ui->listProcessingSteps->setCurrentIndex(index);

	// before config changes, we need to update our redo/undo stacks
	beforeConfigChange();

	// set default name "new step i"
	bool nameAlreadyExists = true;
	int i=0;
    string name = "new step " + std::to_string(i);
	map<string, ProcessingStep> chain = conf_.getProcessingChain();
	while(nameAlreadyExists){
		if (chain.count(name)){
			i++;
			name = "new step " + std::to_string(i);
		} else {
			nameAlreadyExists = false;
		}
	}

	currentStepName = name;

    QString newName = QString::fromStdString(name);
    modelStep->setData(index, newName, Qt::EditRole);

	// add new Processing Step to the configuration chain
    ProcessingStep proSt;
    proSt.name = newName.toStdString();
    conf_.addProcessingStep(proSt);

	// the name can be changed
    ui->listProcessingSteps->edit(index);

}


// updates the name of a step, when changed
void MainWindow::stepNameChanged(){

	// get the new name
	string newName = ui->listProcessingSteps->model()->data(ui->listProcessingSteps->currentIndex()).toString().toStdString();

	// get the old name from the variable currentStepName, which stores the activated step name
	string oldName = currentStepName;

	// checks whether the name has been changed
	if (oldName.compare(newName) != 0){
		// check whether the new name does not already exist
		map<string, ProcessingStep> chain = conf_.getProcessingChain();
		if (!chain.count(newName)){
			beforeConfigChange();

			// create the processing step with the old name and the processing step with the new name
			ProcessingStep proStOld = conf_.getProcessingChain()[oldName];
			ProcessingStep proStNew = conf_.getProcessingChain()[oldName];
			proStNew.name = newName;

			// remove the processing step with the old name and add the processing step with the new name
			conf_.removeProcessingStep(proStOld.name);
			conf_.addProcessingStep(proStNew);
		// name already exists - dont allow the change
		} else{
			modelStep->setData(ui->listProcessingSteps->currentIndex(), QString::fromStdString(oldName), Qt::EditRole);
		}
	}
}


// Delete button clicked
void MainWindow::on_deleteButton_clicked() {
    // Get the position and remove the row
    modelStep->removeRows(ui->listProcessingSteps->currentIndex().row(),1);
    // remove from the chain
	conf_.removeProcessingStep(currentStepName);
}


// gets called when a processing step is selected
void MainWindow::on_listProcessingSteps_activated(const QModelIndex & index) {

	map<string, ProcessingStep> chain = conf_.getProcessingChain();

	string selectedStep = ui->listProcessingSteps->model()->data(ui->listProcessingSteps->currentIndex()).toString().toStdString();
	currentStepName = selectedStep;
	ProcessingStep proStep = chain[selectedStep];

	modelTableParams->setProcessingStep(proStep);
	modelTableInputs->setProcessingStep(proStep);

	// Processing Step Inputs
	model->clear();

	map<string, pair<string, string> > inp = chain[currentStepName].inputs;
	int rowSum = 0;
	if(inp.empty()){
		rowSum = 0;
	} else {
		for (auto it = inp.begin(); it!=inp.end(); ++it) {
			rowSum++;
		}
	}

	model->setColumnCount(2);
	model->setRowCount(rowSum);

	// TODO move this to constructor
	ComboBoxSourceOutput* sourceOutput = new ComboBoxSourceOutput(this);
	ComboBoxSourceStep* sourceStep = new ComboBoxSourceStep(this);


	sourceOutput->setConfiguration(conf_);
	sourceOutput->setCurrentStep(currentStepName);
	sourceOutput->fill();

	sourceStep->setConfiguration(conf_);
	sourceStep->setCurrentStep(currentStepName);
	sourceStep->fill();

	ui->tableView->setItemDelegateForColumn(0, sourceStep);
	ui->tableView->setItemDelegateForColumn(1, sourceOutput);

	// Make the combo boxes always displayed.
	for ( int i = 0; i < model->rowCount(); ++i ) {
		ui->tableView->openPersistentEditor( model->index(i, 1) );
		ui->tableView->openPersistentEditor( model->index(i, 0) );
	}
}

// menu click File -> New
void MainWindow::new_Data_Flow() {
	// save is not activated
	saveAct->setEnabled(false);

	currentFileName = "newFile";
    setWindowTitle(tr((currentFileName + string(" - ") + WINDOW_TITLE).c_str()));

    // configuration changed
	beforeConfigChange();

    Configuration conf;
    conf_ = conf;

    QStringList list;
	modelStep->setStringList(list);
}

void MainWindow::load_Data_Flow()
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."), QString(), tr("YAML-Files (*.yaml);;All Files (*)"));
	// abort button has been pressed
	if (fn.isEmpty()) {
		return;
	}

	loadDataFlow(fn.toStdString());
}

// only possible, if the configuration has already been stored in some file
void MainWindow::save_Data_Flow() {
	conf_.store(currentFileName);
}

// by default the name is the current name of the configuration file
// the suffix of the file is always set to '.yaml'
// the currently opened configuration switches to the stored file
void MainWindow::save_Data_Flow_as() {

	QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                               QString::fromStdString(currentFileName), tr("YAML files (*.yaml);;All Files (*)"));

	// abort button has been pressed
	if (fn.isEmpty()) {
		return;
	}


    if (! (fn.endsWith(".yaml", Qt::CaseInsensitive)) )
        fn += ".yaml"; // default
  	currentFileName = fn.toStdString();
    setWindowTitle(tr((currentFileName + string(" - ") + WINDOW_TITLE).c_str()));
  	saveAct->setEnabled(true);

	conf_.store(fn.toStdString());
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About uipf"),
            tr("This application allows the user by using given library of modules to create his own configuration and to execute it. Created by the project: 'A Unified Framework for Digital Image Processing in Computer Vision and Remote Sensing' at Technische Universitaet Berlin. Version 1.0"));
}

// sets the current configuration to the previous one
void MainWindow::undo() {
	if(!undoStack.empty()){
		// move the current config to the redo stack
		redoStack.push(conf_);
		// get the last config stored in undo stack
		conf_ = undoStack.top();
		// delete the last config from the undo stack
		undoStack.pop();

		// set the names of the processing steps:
		QStringList list;
		map<string, ProcessingStep> chain = conf_.getProcessingChain();
		for (auto it = chain.begin(); it!=chain.end(); ++it) {
			list << it->first.c_str();
		}
		modelStep->setStringList(list);

		// set the undo/redo in the menu bar gray if inactive or black, if active
		redoAct->setEnabled(true);
		if(!undoStack.empty()){
			undoAct->setEnabled(true);
		} else{
			undoAct->setEnabled(false);
		}
	}
}

// sets the current configuration back to the next one
void MainWindow::redo() {
	if(!redoStack.empty()){
		// move the current config to the undo stack
		undoStack.push(conf_);
		// get the last config stored in redo stack
		conf_ = redoStack.top();
		// delete the last config from the redo stack
		redoStack.pop();

		// set the names of the processing steps:
		QStringList list;
		map<string, ProcessingStep> chain = conf_.getProcessingChain();
		for (auto it = chain.begin(); it!=chain.end(); ++it) {
			list << it->first.c_str();
		}
		modelStep->setStringList(list);

		// set the undo/redo in the menu bar gray if inactive or black, if active
		undoAct->setEnabled(true);
		if(!redoStack.empty()){
			redoAct->setEnabled(true);
		} else{
			redoAct->setEnabled(false);
		}
	}
}

// feeds the undo and redo stacks with the current configs
// has to be called BEFORE the config has changed!
void MainWindow::beforeConfigChange(){
	// configuration changed
	undoStack.push(conf_);
	while(! redoStack.empty()){
		redoStack.pop();
	}

	// set the undo to active and redo to inactive
	undoAct->setEnabled(true);
	redoAct->setEnabled(false);
}

// run the current configuration
void MainWindow::run() {
	// stop is now activated
	stopAct->setEnabled(true);

	ModuleManager mm;
	mm.run(conf_);

	// TODO inactivate stop, when finished!
}

void MainWindow::stop() {
	// TODO
}
// Up to here: SLOTS -------------------------------------------------------------------------------------------------------------------------------


// creates available actions
void MainWindow::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new configuration"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(new_Data_Flow()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing configuration"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(load_Data_Flow()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the configuration to disk"));
    saveAct->setEnabled(false); // initially inactive
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save_Data_Flow()));

    saveAsAct = new QAction(tr("Save &as..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the configuration to disk"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(save_Data_Flow_as()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setShortcuts(QKeySequence::WhatsThis);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    undoAct->setEnabled(false); // initially inactive
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    redoAct->setEnabled(false); // initially inactive
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    runAct = new QAction(tr("&Run"), this);
    // TODO set shortcut
    runAct->setStatusTip(tr("Run the configuration"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(run()));

    stopAct = new QAction(tr("&Stop"), this);
    // TODO set shortcut
    stopAct->setStatusTip(tr("Stop the execution of the configuration"));
    stopAct->setEnabled(false); // initially inactive
    connect(stopAct, SIGNAL(triggered()), this, SLOT(stop()));
}

// delete actions from heap for destructor
void MainWindow::deleteActions() {
    delete newAct;
    delete openAct;
    delete saveAct;
    delete saveAsAct;
    delete exitAct;
    delete aboutAct;
    delete undoAct;
    delete redoAct;
    delete runAct;
    delete stopAct;
}

// creates the menu bar and attaches the actions
void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    configMenu = menuBar()->addMenu(tr("&Configuration"));
    configMenu->addAction(runAct);
    configMenu->addAction(stopAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
