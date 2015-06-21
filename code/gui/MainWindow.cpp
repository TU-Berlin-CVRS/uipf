#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <iostream>
#include "../framework/ModuleManager.hpp"

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

	// Processing Step Names:
    // allow to manually modify the data 
    // -> It may be triggered by hitting any key or double-click etc.
    ui->listProcessingSteps-> setEditTriggers(QAbstractItemView::AnyKeyPressed |QAbstractItemView::DoubleClicked);
	// react to selection of the entries
	// -> TODO improve this to react on any selection change: http://stackoverflow.com/questions/2468514/how-to-get-the-selectionchange-event-in-qt
    connect(ui->listProcessingSteps, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(on_listProcessingSteps_activated(const QModelIndex &)));
    // react to changes in the entries    
    connect(modelStep, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
            this, SLOT(stepNameChanged()));
            
                    
    // commands for menu bar     
    createActions();
    createMenus();
	// sets undo and redo to inactive
    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    // sets run and stop to inactive
    runAct->setEnabled(false);
    stopAct->setEnabled(false);
    // sets save to inactive 
    saveAct->setEnabled(false);
    
    
    // window settings
    setWindowTitle(tr("uipf"));
    setMinimumSize(800, 600);
    resize(1200, 800);

	// logger
    connect(Logger::instance(), SIGNAL (logEvent(const Logger::LogType&,const std::string&)), 
			this, SLOT (on_appendToLog(const Logger::LogType&,const std::string&)));
    
    // when starting the program, at first always start a new Data Flow
   	new_Data_Flow();
}

// destructor
MainWindow::~MainWindow() {
    delete ui;
    delete modelStep;
    delete modelModule;
    delete modelTableParams;
    delete modelTableInputs;
}


// sets a Module list
/*
list	list of all availanle modules 
*/
void MainWindow::setModuleList(QStringList list){
	modelModule->setStringList(list);
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
	configChanged();

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
			configChanged();
			
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
}

void MainWindow::new_Data_Flow() {
	// save is not activated
	saveAct->setEnabled(false);    
	// run is now activated
	runAct->setEnabled(true);    
	
	currentFileName = "newFile";
    setWindowTitle(tr("newFile - uipf"));
	
	
    // configuration changed
	configChanged();

    Configuration conf;
    conf_ = conf;
    
    QStringList list;
	modelStep->setStringList(list);
}




void MainWindow::load_Data_Flow() {
	// run is now activated
	runAct->setEnabled(true);    

    // configuration changed
	configChanged();

	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),QString(), tr("YAML-Files (*.yaml);;All Files (*)"));
  	currentFileName = fn.toStdString();
  	string winTitle = currentFileName + " - uipf";
    setWindowTitle(tr(winTitle.c_str()));

  	
  	saveAct->setEnabled(true);

	conf_.load(currentFileName);

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
                                              
    if (! (fn.endsWith(".yaml", Qt::CaseInsensitive)) )
        fn += ".yaml"; // default
  	currentFileName = fn.toStdString();
  	string winTitle = currentFileName + " - uipf";
    setWindowTitle(tr(winTitle.c_str()));
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
void MainWindow::configChanged(){
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
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    runAct = new QAction(tr("&Run"), this);
    runAct->setStatusTip(tr("Run the configuration"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(run()));

    stopAct = new QAction(tr("&Stop"), this);
    stopAct->setStatusTip(tr("Stop the execution of the configuration"));
    connect(stopAct, SIGNAL(triggered()), this, SLOT(stop()));
}

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

