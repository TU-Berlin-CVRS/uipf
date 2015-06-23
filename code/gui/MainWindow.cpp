#include <QScrollBar>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardItemModel>

#include <iostream>

#include "../framework/ModuleManager.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

using namespace std;
using namespace uipf;

// constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // Create models
    modelStep = new QStringListModel(this);
    modelTableParams = new ProcessingStepParams(this);
    modelTableInputs = new QStandardItemModel(this);
	modelSourceOutput = new ComboBoxSourceOutput(mm_, this);
	modelSourceStep = new ComboBoxSourceStep(this);

    // Glue model and view together
    ui->listProcessingSteps->setModel(modelStep);
    ui->tableParams->setModel(modelTableParams);
    ui->tableInputs->setModel(modelTableInputs);

	//create and add the graphwidget to the gui
	graphView_ = new gui::GraphWidget();
	ui->verticalLayoutRight->addWidget(graphView_);//add graphview

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
            this, SLOT(on_stepNameChanged()));
    // react to changes in the module
    connect(ui->comboModule, SIGNAL(currentIndexChanged(int)),
			this, SLOT(on_comboModule_currentIndexChanged(int)));
	// react to changes in the params
    connect(modelTableParams, SIGNAL(paramChanged(std::string, std::string)),
			this, SLOT(on_paramChanged(std::string, std::string)));
	// logger
    connect(Logger::instance(), SIGNAL (logEvent(const Logger::LogType&,const std::string&)),
			this, SLOT (on_appendToLog(const Logger::LogType&,const std::string&)));

	// fill module dropdown
	map<string, MetaData> modules = mm_.getAllModuleMetaData();
	int mi = 0;
	for (auto it = modules.begin(); it!=modules.end(); ++it) {
		ui->comboModule->insertItem(mi++, QString(it->first.c_str()), QString(it->first.c_str()));
	}
	ui->comboModule->setCurrentIndex(-1);

    // commands for menu bar
    createActions();
    createMenus();

    // window settings
    setMinimumSize(800, 600);
    resize(1200, 800);


    // set initial state
	currentFileName = "newFile";
    setWindowTitle(tr((currentFileName + string(" - ") + WINDOW_TITLE).c_str()));
    ui->comboModule->setEnabled(false);
	ui->tableParams->setEnabled(false);
	ui->tableInputs->setEnabled(false);
}

// destructor
MainWindow::~MainWindow() {
    delete ui;
    delete modelStep;
    delete modelTableParams;
    delete modelTableInputs;
    delete graphView_;

    deleteActions();
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

	// reset other models
	resetModule();
	resetParams();
	resetInputs();
	//update the graphview
	refreshGraph();
}

// refresh module dropdown
void MainWindow::refreshModule()
{
    ui->comboModule->setEnabled(true);

	ProcessingStep step = conf_.getProcessingStep(currentStepName);

	// set the selection of the module dropdown
	int modIndex = ui->comboModule->findData(QString(step.module.c_str()));
	ui->comboModule->setCurrentIndex(modIndex);
}

// refresh params table
void MainWindow::refreshParams()
{
	ui->tableParams->setEnabled(true);

	ProcessingStep step = conf_.getProcessingStep(currentStepName);
	modelTableParams->setProcessingStep(step);

	// ensure size of the columns match the widget
	for (int c = 0; c < ui->tableParams->horizontalHeader()->count(); ++c) {
		ui->tableParams->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
	}

}

// refresh inputs table
void MainWindow::refreshInputs()
{
	ui->tableInputs->setEnabled(true);

	ProcessingStep step = conf_.getProcessingStep(currentStepName);

	modelTableInputs->clear();

	map<string, pair<string, string> > inp = step.inputs;
	int rowCount = 0;
	vector<QStandardItem*> items;
	for (auto it = inp.begin(); it!=inp.end(); ++it) {
		items.push_back(new QStandardItem((it->first).c_str()));
		rowCount++;
	}

	modelTableInputs->setRowCount(rowCount);

	// TODO this could be constant somewhere
	modelTableInputs->setColumnCount(2);
	QStandardItem* item0 = new QStandardItem("From Step:");
	QStandardItem* item1 = new QStandardItem("Output Name:");
	modelTableInputs->setHorizontalHeaderItem(0, item0);
	modelTableInputs->setHorizontalHeaderItem(1, item1);
	// TODO until here

	for (unsigned int i = 0; i<items.size(); i++){
		modelTableInputs->setVerticalHeaderItem(i, items[i]);
	}

	//~ ui->tableInputs->repaint();

	modelSourceStep->setConfiguration(conf_, currentStepName);
	modelSourceOutput->setConfiguration(conf_, currentStepName);


	ui->tableInputs->setItemDelegateForColumn(0, modelSourceStep);
	ui->tableInputs->setItemDelegateForColumn(1, modelSourceOutput);

	// Make the combo boxes always displayed.
	for ( int i = 0; i < modelTableInputs->rowCount(); ++i ) {
		ui->tableInputs->openPersistentEditor( modelTableInputs->index(i, 1) );
		ui->tableInputs->openPersistentEditor( modelTableInputs->index(i, 0) );
	}

	// ensure size of the columns match the widget
	for (int c = 0; c < ui->tableInputs->horizontalHeader()->count(); ++c) {
		ui->tableInputs->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
	}
}

// refresh the graph view
void MainWindow::refreshGraph()
{
    graphView_->renderConfig(conf_);
}

// resets the module dropdown to empty
void MainWindow::resetModule()
{
	ui->comboModule->setCurrentIndex(-1);
    ui->comboModule->setEnabled(false);
}

// resets the params table to empty
void MainWindow::resetParams()
{
	ProcessingStep step;
	step.params.insert(pair<string, string>("", ""));
	modelTableParams->setProcessingStep(step);
	ui->tableParams->setEnabled(false);

	// ensure size of the columns match the widget
	for (int c = 0; c < ui->tableParams->horizontalHeader()->count(); ++c) {
		ui->tableParams->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
	}
}

// resets the inputs table to empty
void MainWindow::resetInputs()
{
	ProcessingStep step;
	modelTableInputs->clear();
	ui->tableInputs->setEnabled(false);
}



// From here: SLOTS -------------------------------------------------------------------------------------------------------------------------------

// append messages from our logger to the log-textview
void MainWindow::on_appendToLog(const Logger::LogType& eType,const std::string& strText) {
	// For colored Messages we need html :-/
	QString strColor = (eType == Logger::WARNING ? "Blue" : eType == Logger::ERROR ? "Red" : "Green");
	QString alertHtml = "<font color=\""+strColor+"\">" + QString(strText.c_str()) + "</font>";
	ui->tbLog->appendHtml(alertHtml);
	//autoscroll
	ui->tbLog->verticalScrollBar()->setValue(ui->tbLog->verticalScrollBar()->maximum());
}


// Add button clicked - allows to add a new Processing Step
// Step is always added at the end of the list
void MainWindow::on_addButton_clicked() {
    // Get the position of the selected item
    int row = modelStep->rowCount();

    // Enable add one or more rows
    modelStep->insertRows(row, 1);

    // Get the row for Edit mode
    QModelIndex index = modelStep->index(row);

    // Enable item selection
    ui->listProcessingSteps->setCurrentIndex(index);

	// before config changes, we need to update our redo/undo stacks
	beforeConfigChange();

	// set default name "new step i"
	bool nameAlreadyExists = true;
	int i=1;
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

	// put selected item in edit mode
    ui->listProcessingSteps->edit(index);

	// refresh configuration widgets
	refreshModule();
	refreshInputs();
	refreshParams();
    // update the graphview
    refreshGraph();
}


// updates the name of a step, when changed
void MainWindow::on_stepNameChanged(){

	// get the new name
	string newName = ui->listProcessingSteps->model()->data(ui->listProcessingSteps->currentIndex()).toString().toStdString();

	// get the old name from the variable currentStepName, which stores the activated step name
	string oldName = currentStepName;

	// checks whether the name has been changed
	if (oldName.compare(newName) == 0) {
		return;
	}

	// check whether the new name does not already exist
	if (!conf_.hasProcessingStep(newName)) {
		beforeConfigChange();

		conf_.renameProcessingStep(oldName, newName);

		// update current step pointer
		currentStepName = newName;
	} else {
		// name already exists - dont allow the change
		modelStep->setData(ui->listProcessingSteps->currentIndex(), QString::fromStdString(oldName), Qt::EditRole);
	}

	//update the graphview
	refreshGraph();
}


// Delete button clicked
void MainWindow::on_deleteButton_clicked() {
    // Get the position and remove the row
    modelStep->removeRows(ui->listProcessingSteps->currentIndex().row(),1);
    // remove from the chain
	conf_.removeProcessingStep(currentStepName);

	//update the graphview
	refreshGraph();

	// ensure configuration widgets are empty
	resetModule();
	resetParams();
	resetInputs();
}


// gets called when a processing step is selected
void MainWindow::on_listProcessingSteps_activated(const QModelIndex & index)
{
	currentStepName = ui->listProcessingSteps->model()->data(ui->listProcessingSteps->currentIndex()).toString().toStdString();

	// refresh configuration widgets
	refreshModule();
	refreshParams();
	refreshInputs();
}

void MainWindow::on_comboModule_currentIndexChanged(int index)
{
	string module = ui->comboModule->itemData(index).toString().toStdString();

	// only update if module exists, a step is selected and the module has actually changed
	if (mm_.hasModule(module) && !currentStepName.empty() && conf_.getProcessingStep(currentStepName).module.compare(module) != 0) {
		beforeConfigChange();
		conf_.setProcessingStepModule(currentStepName, module, mm_.getModuleMetaData(module));
		refreshParams();
		refreshInputs();
	}
}

// react to params changes and store them in the config
void MainWindow::on_paramChanged(std::string paramName, std::string value)
{
	if (!currentStepName.empty()) {
		beforeConfigChange();
		map<string, string> params = conf_.getProcessingStep(currentStepName).params;
		params[paramName] = value;
		conf_.setProcessingStepParams(currentStepName, params);
	}
}



// menu click File -> New
void MainWindow::new_Data_Flow()
{
	if (currentFileHasChanged) {
		// TODO confirm if the current data should really be dropped
	}

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

	// reset other models
	resetModule();
	resetParams();
	resetInputs();
	// update the graphview
	refreshGraph();

}

void MainWindow::load_Data_Flow()
{
	if (currentFileHasChanged) {
		// TODO confirm if the current data should really be dropped
	}

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
	currentFileHasChanged = false;
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
	currentFileHasChanged = false;

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

		// refresh configuration widgets
		refreshModule();
		refreshInputs();
		refreshParams();
		// update the graphview
		refreshGraph();
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

		// refresh configuration widgets
		refreshModule();
		refreshInputs();
		refreshParams();
		// update the graphview
		refreshGraph();
	}
}

// feeds the undo and redo stacks with the current configs
// has to be called BEFORE the config has changed!
void MainWindow::beforeConfigChange(){
	// configuration changed
	currentFileHasChanged = true;

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

	// validate configuration and show errors
	vector<string> errors = conf_.validate(mm_.getAllModuleMetaData());
	if (!errors.empty()) {
		LOG_E("There are configuration errors!");
		for(unsigned int i = 0; i < errors.size(); ++i) {
			LOG_E( errors[i]);
		}
		return;
	}

	// stop is now activated and run unactivated
	stopAct->setEnabled(true);
	runAct->setEnabled(false);

	mm_.run(conf_);

	// run is now activated and stop unactivated
	stopAct->setEnabled(false);
	runAct->setEnabled(true);
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
