#include <QScrollBar>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardItemModel>
#include <QThread>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QModelIndex>
#include <QPointer>
#include <QShortcut>
#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>

#include "MainWindow.hpp"
#include "ImageWindow.hpp"
#include "ui_mainwindow.h"
#include "../framework/GUIEventDispatcher.hpp"
#include "RunWorkerThread.h"

using namespace std;
using namespace uipf;

// constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), workerThread_(nullptr) {

    ui->setupUi(this);

    mm_.setHaveGUI();

    // Create models

    // create model for step list
    modelStep = new QStringListModel(this);
    // create model for params list
    modelTableParams = new ParamsModel(mm_,this);

    // create model for inputs list
    modelTableInputs = new QStandardItemModel(this);
	modelTableInputs->setColumnCount(2);
	QStandardItem* item0 = new QStandardItem("From Step:");
	QStandardItem* item1 = new QStandardItem("Output Name:");
	item0->setToolTip(QString("Select here the processing step, which provides the needed input"));
	item1->setToolTip(QString("Select here the output of the selected processing step, which is needed as input"));
	modelTableInputs->setHorizontalHeaderItem(0, item0);
	modelTableInputs->setHorizontalHeaderItem(1, item1);
	delegateTableInputs = new InputsDelegate(mm_, this);

    // Glue model and view together
    ui->listProcessingSteps->setModel(modelStep);
    ui->tableParams->setModel(modelTableParams);
    ui->tableInputs->setModel(modelTableInputs);
	ui->tableInputs->setItemDelegateForColumn(0, delegateTableInputs);
	ui->tableInputs->setItemDelegateForColumn(1, delegateTableInputs);
	// ensure size of the columns match the widget
	for (int c = 0; c < ui->tableInputs->horizontalHeader()->count(); ++c) {
		ui->tableInputs->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
	}
	ui->tableInputs->setEditTriggers(QAbstractItemView::AllEditTriggers); // http://doc.qt.io/qt-5/qabstractitemview.html#EditTrigger-enum

	//create and add the graphwidget to the gui
	graphView_ = new gui::GraphWidget();
	ui->verticalLayoutRight->addWidget(graphView_);//add graphview

	// Processing Step Names: allow to manually modify the data
    // -> It may be triggered by hitting any key or double-click etc.
    ui->listProcessingSteps-> setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);

    ui->progressBar->setValue(0.0f);
    ui->progressBar->setHidden(true);
	// set up slots for signals

	// react to selection of the entries
    connect(ui->listProcessingSteps->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			this, SLOT(on_stepSelectionChanged(QItemSelection)));

	QShortcut *shortcut = new QShortcut(QKeySequence("Del"), this);
	ui->clearLogButton->connect(shortcut, SIGNAL(activated()),
			this, SLOT(on_clearLogButton_clicked()));


	ui->deleteButton->connect(shortcut, SIGNAL(activated()),
				this, SLOT(on_deleteButton_clicked()));

    // react to changes in the entries
    connect(modelStep, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
            this, SLOT(on_stepNameChanged()));
    // react to changes in the module category
    connect(ui->comboCategory, SIGNAL(currentIndexChanged(int)),
			this, SLOT(on_comboCategory_currentIndexChanged(int)));
    // react to changes in the module
    connect(ui->comboModule, SIGNAL(currentIndexChanged(int)),
			this, SLOT(on_comboModule_currentIndexChanged(int)));
	// react to changes in the params
    connect(modelTableParams, SIGNAL(paramChanged(std::string, std::string)),
			this, SLOT(on_paramChanged(std::string, std::string)));
	// react to changes in the inputs
    connect(delegateTableInputs, SIGNAL(inputChanged(std::string, std::pair<std::string, std::string>)),
			this, SLOT(on_inputChanged(std::string, std::pair<std::string, std::string>)));
    // logger
    connect(GUIEventDispatcher::instance(), SIGNAL (logEvent(const Logger::LogType&,const std::string&)),
			this, SLOT (on_appendToLog(const Logger::LogType&,const std::string&)));

    // progressbar
    connect(GUIEventDispatcher::instance(), SIGNAL (reportProgressEvent(const float&)),
    			this, SLOT (on_reportProgress(const float&)));

    //synchronize selection in graph and combobox
    connect(graphView_, SIGNAL (nodeSelected(const uipf::gui::Node*)),
       			this, SLOT (on_graphNodeSelected(const uipf::gui::Node*)));


    // Window creation
    connect(GUIEventDispatcher::instance(), SIGNAL (createWindow(const std::string&)),
				this, SLOT (on_createWindow(const std::string&)));

    // Window deletion
    connect(GUIEventDispatcher::instance(), SIGNAL (closeWindow(const std::string&)),
    			this, SLOT (on_closeWindow(const std::string&)));

    //Filter logwindow
    connect(ui->logFilterLE, SIGNAL (textChanged(const QString &)),
    				this, SLOT (on_logFiltertextChanged(const QString&)));


	// fill module categories dropdown
	map<string, MetaData> modules = mm_.getAllModuleMetaData();
	for (auto it = modules.begin(); it!=modules.end(); ++it) {
		if(categories_.count(it->second.getCategory()) == 0){
			vector<string> temp;
			temp.push_back(it->first);
			categories_.insert( pair<string, vector<string> > (it->second.getCategory(), temp));
		} else {
			categories_[it->second.getCategory()].push_back(it->first);
		}
	}
	int mi = 0;
	for (auto it = categories_.begin(); it!=categories_.end(); ++it) {
		ui->comboCategory->insertItem(mi++, QString(it->first.c_str()), QString(it->first.c_str()));
	}
	ui->comboCategory->setCurrentIndex(-1);

    // commands for menu bar
    createActions();
    createMenus();

    // window settings
    setMinimumSize(800, 600);
    resize(1200, 800);


    // set initial state
	currentFileName = "newFile";
    setWindowTitle(tr((currentFileName + string(" - ") + WINDOW_TITLE).c_str()));
    ui->comboCategory->setEnabled(false);
    ui->comboModule->setEnabled(false);
	ui->tableParams->setEnabled(false);
	ui->tableInputs->setEnabled(false);
	ui->deleteButton->setEnabled(false);
	closeWindowsAct->setEnabled(false);

	resetParams();

	refreshSaveIcon();

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

void MainWindow::closeAllCreatedWindows()
{
	//close all windows we created
	for (auto w : createdWindwows_)
	{
		if (w != nullptr)
		{
			w->close();
		}
	}
	createdWindwows_.clear();

	closeWindowsAct->setEnabled(false);

}

// this event gets fired when the window gets closed
void MainWindow::closeEvent(QCloseEvent *event)
{
	// check whether there are unsaved changes, and ask the user, whether he wants to save them
    if (okToContinue()) {
		closeAllCreatedWindows();
        event->accept();
    } else {
        event->ignore();
    }
}

// loads a new configuration from file
void MainWindow::loadDataFlow(string filename)
{

	ui->deleteButton->setEnabled(false);
	unknownFile = false;
	savedVersion = 0;
	refreshSaveIcon();

	// when new dataflow, both stacks become empty and the buttons will be deactivated
	while(! redoStack.empty()){
		redoStack.pop();
	}
	while(! undoStack.empty()){
		undoStack.pop();
	}
	redoAct->setEnabled(false);
	undoAct->setEnabled(false);

	currentFileName = filename;
    setWindowTitle(tr((currentFileName + string(" - ") + WINDOW_TITLE).c_str()));

	conf_.load(currentFileName);

    // only for debug, print the loaded config
	//conf_.print();

	// save is now activated
	saveAct->setEnabled(false);

	// set the names of the processing steps:
	QStringList list;
	map<string, ProcessingStep> chain = conf_.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		list << it->first.c_str();
	}
	modelStep->setStringList(list);

	// reset other models
	resetCategoryAndModule();
	resetParams();
	resetInputs();
	//update the graphview
	refreshGraph();
}

// refresh module category dropdown and module dropdown
void MainWindow::refreshCategoryAndModule()
{
    if (currentStepName.empty() || !conf_.hasProcessingStep(currentStepName)){
		resetCategoryAndModule();
		return;
	}

    ui->comboCategory->setEnabled(true);
    ui->comboModule->setEnabled(true);

	ProcessingStep step = conf_.getProcessingStep(currentStepName);
	if (step.module.empty() || !mm_.hasModule(step.module)){
		resetCategoryAndModule();
		ui->comboCategory->setEnabled(true);
		return;
	}
	int modIndex = ui->comboModule->findData(QString(step.module.c_str()));

	// set the selection of the module category dropdown
	int catIndex = ui->comboCategory->findData(QString(mm_.getModuleMetaData(step.module).getCategory().c_str()));
	ui->comboCategory->setCurrentIndex(catIndex);
	on_comboCategory_currentIndexChanged(catIndex);

	// set the selection of the module dropdown
	modIndex = ui->comboModule->findData(QString(step.module.c_str()));
	ui->comboModule->setCurrentIndex(modIndex);

}

// refresh params table
void MainWindow::refreshParams()
{
	if (currentStepName.empty() || !conf_.hasProcessingStep(currentStepName)){
		resetParams();
		return;
	}

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
	if (currentStepName.empty() || !conf_.hasProcessingStep(currentStepName)){
		resetInputs();
		return;
	}

	ui->tableInputs->setEnabled(true);

	ProcessingStep step = conf_.getProcessingStep(currentStepName);

	// clear all data rows
	modelTableInputs->setRowCount(0);

	map<string, pair<string, string> > inputs = step.inputs;
	vector<string> inputNames(inputs.size());
	int row = 0;
	for (auto it = inputs.begin(); it!=inputs.end(); ++it) {
		QStandardItem* item = new QStandardItem((it->first).c_str());
		if (mm_.hasModule(step.module)) {
			map<string, DataDescription> inputs = mm_.getModuleMetaData(step.module).getInputs();
			if (inputs.count(it->first) > 0) {
				string str = inputs[it->first].getDescription();
				item->setToolTip(QString(str.c_str()));
			}
		}
		modelTableInputs->setVerticalHeaderItem(row, item);
		inputNames[row] = it->first;
		modelTableInputs->setItem(row, 0, new QStandardItem((it->second.first).c_str()));
		modelTableInputs->setItem(row, 1, new QStandardItem((it->second.second).c_str()));
		row++;
	}

	delegateTableInputs->setConfiguration(conf_, currentStepName, inputNames);
}

// refresh save icon
void MainWindow::refreshSaveIcon()
{
	QPixmap image;
    if (savedVersion == 0) {
		image = QPixmap(":/images/pen.png");
	} else {
		image = QPixmap(":/images/penRed.png");
	}
	QIcon icon(image);
	icon.addPixmap(image, QIcon::Mode::Disabled, QIcon::State::On);
	icon.addPixmap(image, QIcon::Mode::Disabled, QIcon::State::Off);
	ui->status->setIcon(icon);
}

// refresh the graph view
void MainWindow::refreshGraph()
{
    graphView_->renderConfig(conf_);
}

// resets the category dropdown and the module dropdown to empty
void MainWindow::resetCategoryAndModule()
{
	ui->comboCategory->setCurrentIndex(-1);
    ui->comboCategory->setEnabled(false);

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
	// clear all data rows
	modelTableInputs->setRowCount(0);
	ui->tableInputs->setEnabled(false);
}



// From here: SLOTS -------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_createWindow(const std::string& strTitle)
{
	// fetch the image from the GUIEventDispatcher
	QImage image = GUIEventDispatcher::instance()->image_;

	//simple view that contains an Image
	QPointer<QGraphicsScene> scene = new QGraphicsScene;
	QPointer<QGraphicsView> view = new ImageWindow(mm_, scene);
	view->setWindowTitle( QString::fromStdString(strTitle));
	QPixmap pixmap = QPixmap::fromImage(image);
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
	scene->addItem(item);
	view->show();
	createdWindwows_.push_back(view);

	closeWindowsAct->setEnabled(true);

	// unlock the mutex with the working thread
	GUIEventDispatcher::instance()->imageRendered.wakeAll();
}

void MainWindow::on_closeWindow(const std::string& strTitle)
{
	auto qTitle = QString::fromStdString(strTitle);
	for (auto v : createdWindwows_)
	{
		if (v->windowTitle() == qTitle)
			v->close();
	}
}

// append messages from our logger to the log-textview
void MainWindow::on_appendToLog(const Logger::LogType& eType,const std::string& strText) {

	if (!ui->logWarnCheckbox->isChecked() && eType == Logger::WARNING )return;
	if (!ui->logInfoCheckbox->isChecked() && eType == Logger::INFO )return;
	if (!ui->logErrorCheckbox->isChecked() && eType == Logger::ERROR )return;

	QString qText = QString(strText.c_str());

	//filter
	if (!qText.toLower().contains(ui->logFilterLE->text().toLower()))return;

	// For colored Messages we need html :-/
	QString strColor = (eType == Logger::WARNING ? "Blue" : eType == Logger::ERROR ? "Red" : "Green");
	QString alertHtml = "<font color=\""+strColor+"\">" + qText + "</font>";
	ui->tbLog->appendHtml(alertHtml);
	//autoscroll
	ui->tbLog->verticalScrollBar()->setValue(ui->tbLog->verticalScrollBar()->maximum());
}

void MainWindow::on_clearLogButton_clicked()
{
	ui->tbLog->clear();
}

void MainWindow::on_logFiltertextChanged(const QString& text)
{
	//2DO Filter on demand
	//some kind of view is needed to display only messages fitting the filter
	//old messages must be stored to be displayed again when filter is removed
}

// moves the progressbar on every step of the processing chain
void MainWindow::on_reportProgress(const float& fValue)
{
	ui->progressBar->setValue(fValue);
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
	ui->deleteButton->setEnabled(true);

    QString newName = QString::fromStdString(name);
    modelStep->setData(index, newName, Qt::EditRole);

	// add new Processing Step to the configuration chain
    ProcessingStep proSt;
    proSt.name = newName.toStdString();
    conf_.addProcessingStep(proSt);

	// put selected item in edit mode
    ui->listProcessingSteps->edit(index);

	// refresh configuration widgets
	refreshCategoryAndModule();
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
	if (!ui->deleteButton->isEnabled()) return;
    // remove from the chain
	beforeConfigChange();

	// remove the step from the configuration and reset step name
	conf_.removeProcessingStep(currentStepName);
	currentStepName = string("");

    // Get the position and remove the row
    modelStep->removeRows(ui->listProcessingSteps->currentIndex().row(),1);

	// if there is a processing step selected, set currentStepName
	if (ui->listProcessingSteps->currentIndex().row() == -1){
		ui->deleteButton->setEnabled(false);
	} else {
		currentStepName = ui->listProcessingSteps->model()->data(ui->listProcessingSteps->currentIndex()).toString().toStdString();
	}


	// refresh configuration widgets
	refreshCategoryAndModule();
	refreshInputs();
	refreshParams();
    // update the graphview
    refreshGraph();
}

void MainWindow::on_graphNodeSelected(const uipf::gui::Node* node)
{
	int row = modelStep->stringList().indexOf(node->name_);

	QModelIndex index = modelStep->index(row, 0);
	ui->listProcessingSteps->setCurrentIndex(index);
}

// gets called when a processing step is selected
void MainWindow::on_stepSelectionChanged(const QItemSelection& selection){
	if(selection.indexes().isEmpty()) {
		ui->deleteButton->setEnabled(false);
		resetCategoryAndModule();
		resetParams();
		resetInputs();
	} else {
		ui->deleteButton->setEnabled(true);
		ui->listProcessingSteps->setCurrentIndex(selection.indexes().first());
		currentStepName = ui->listProcessingSteps->model()->data(ui->listProcessingSteps->currentIndex()).toString().toStdString();
		// refresh configuration widgets
		refreshCategoryAndModule();
		refreshParams();
		refreshInputs();
		graphView_->selectNodeByName(QString::fromStdString(currentStepName));
	}
}

void MainWindow::on_comboCategory_currentIndexChanged(int index)
{
	// only update if a step is selected
	if (!currentStepName.empty()) {

		string category = ui->comboCategory->itemData(index).toString().toStdString();

		map<string, MetaData> modules = mm_.getAllModuleMetaData();

		// fill module dropdown
		ui->comboModule->setEnabled(false);
		ui->comboModule->clear();
		vector<string> modulesOfSameCategory = categories_[category];
		for (unsigned int i = 0; i < modulesOfSameCategory.size() ; i++) {
			ui->comboModule->insertItem(i, QString(modulesOfSameCategory[i].c_str()), QString(modulesOfSameCategory[i].c_str()));
			string str = mm_.getModuleMetaData(modulesOfSameCategory[i]).getDescription();
			ui->comboModule->setItemData(i, QVariant(str.c_str()) , Qt::ToolTipRole);
		}
		ui->comboModule->setCurrentIndex(-1);
		ui->comboModule->setEnabled(true);


	}

}

void MainWindow::on_comboModule_currentIndexChanged(int index)
{
	string module = ui->comboModule->itemData(index).toString().toStdString();

	if (ui->comboModule->isEnabled() // combo was enabled (ensure it was changed by user and not when filling the box)
	 && !currentStepName.empty() // a step is selected
	 && mm_.hasModule(module) // only update if module exists
	 && conf_.hasProcessingStep(currentStepName)
	 && conf_.getProcessingStep(currentStepName).module.compare(module) != 0 // the module has actually changed
	) {
		beforeConfigChange();
		conf_.setProcessingStepModule(currentStepName, module, mm_.getModuleMetaData(module));
		refreshParams();
		refreshInputs();
		refreshGraph();
	}
}

// react to params changes and store them in the config
void MainWindow::on_paramChanged(std::string paramName, std::string value)
{
	if (!currentStepName.empty() && conf_.hasProcessingStep(currentStepName)) {
		beforeConfigChange();
		map<string, string> params = conf_.getProcessingStep(currentStepName).params;
		params[paramName] = value;
		conf_.setProcessingStepParams(currentStepName, params);
		refreshGraph();
	}
}

// react to input changes and store them in the config
void MainWindow::on_inputChanged(std::string inputName, std::pair<std::string, std::string> value)
{
	if (!currentStepName.empty() && conf_.hasProcessingStep(currentStepName)) {
		beforeConfigChange();
		map<string, pair<string, string> > inputs = conf_.getProcessingStep(currentStepName).inputs;
		inputs[inputName] = value;
		conf_.setProcessingStepInputs(currentStepName, inputs);

		refreshInputs();
		refreshGraph();
	}
}


// menu click File -> Exit
void MainWindow::on_close() {

	// close the window, saved file is checked in close event handler
	close();

}


// menu click File -> New
void MainWindow::new_Data_Flow() {
	//check whether there are unsaved changes, and ask the user, whether he wants to save them
	if (!okToContinue()) return;

	ui->deleteButton->setEnabled(false);
	unknownFile = true;

	savedVersion = 1;
	refreshSaveIcon();

	// when new dataflow, both stacks become empty and the buttons will be deactivated
	while(! redoStack.empty()){
		redoStack.pop();
	}
	while(! undoStack.empty()){
		undoStack.pop();
	}
	redoAct->setEnabled(false);
	undoAct->setEnabled(false);

	// save is not activated
	saveAct->setEnabled(false);

	currentFileName = "newFile";
	setWindowTitle(tr((currentFileName + string(" - ") + WINDOW_TITLE).c_str()));

	Configuration conf;
	conf_ = conf;

	QStringList list;
	modelStep->setStringList(list);

	// reset other models
	resetCategoryAndModule();
	resetParams();
	resetInputs();
	// update the graphview
	refreshGraph();

}

void MainWindow::load_Data_Flow()
{
	//check whether there are unsaved changes, and ask the user, whether he wants to save them
	if (!okToContinue()) return;

	QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."), QString(), tr("YAML-Files (*.yaml);;All Files (*)"));

	// if abort button has been pressed
	if (fn.isEmpty()) {
		return;
	}

	loadDataFlow(fn.toStdString());
}

// when unsaved changes occure, give the user the possibility to save them
bool MainWindow::okToContinue() {

    if (savedVersion != 0 && undoAct->isEnabled()) {
        int r = QMessageBox::warning(this, tr("Spreadsheet"),
                        tr("The document has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
			if (saveAct->isEnabled() ) save_Data_Flow();
            else save_Data_Flow_as();
            return true;
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}


// only possible, if the configuration has already been stored in some file
void MainWindow::save_Data_Flow() {
	conf_.store(currentFileName);
	unknownFile = false;
	savedVersion = 0;
	refreshSaveIcon();
	saveAct->setEnabled(false);

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

	conf_.store(currentFileName);

	saveAct->setEnabled(false);
	unknownFile = false;
	savedVersion = 0;
	refreshSaveIcon();

}

void MainWindow::about() {
    QMessageBox::about(this, tr("About uipf"),
            tr("This application allows the user by using given library of modules to create his own configuration and to execute it. Created by the project: 'A Unified Framework for Digital Image Processing in Computer Vision and Remote Sensing' at Technische Universitaet Berlin. Version 1.0"));
}

// sets the current configuration to the previous one
void MainWindow::undo() {
	if(!undoStack.empty()){

		if (!unknownFile) saveAct->setEnabled(true);


		savedVersion--;
		if (!unknownFile && savedVersion == 0){
			saveAct->setEnabled(false);
		}
		refreshSaveIcon();


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

		// reset current step if necessary
		if (!conf_.hasProcessingStep(currentStepName)) {
			currentStepName = string("");
			ui->deleteButton->setEnabled(false);
		}

		// set the undo/redo in the menu bar gray if inactive or black, if active
		redoAct->setEnabled(true);
		if(!undoStack.empty()){
			undoAct->setEnabled(true);
		} else{
			undoAct->setEnabled(false);
		}

		// refresh configuration widgets
		refreshCategoryAndModule();
		refreshInputs();
		refreshParams();
		// update the graphview
		refreshGraph();
	}
}

// sets the current configuration back to the next one
void MainWindow::redo() {

	if(!redoStack.empty()){

		if (!unknownFile) saveAct->setEnabled(true);


		savedVersion++;
		if (!unknownFile && savedVersion == 0){
			saveAct->setEnabled(false);
		}
		refreshSaveIcon();


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

		// reset current step if necessary
		if (!conf_.hasProcessingStep(currentStepName)) {
			currentStepName = string("");
			ui->deleteButton->setEnabled(false);
		}

		// set the undo/redo in the menu bar gray if inactive or black, if active
		undoAct->setEnabled(true);
		if(!redoStack.empty()){
			redoAct->setEnabled(true);
		} else{
			redoAct->setEnabled(false);
		}

		// refresh configuration widgets
		refreshCategoryAndModule();
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
	savedVersion++;
	if (!unknownFile) saveAct->setEnabled(true);

	undoStack.push(conf_);
	while(! redoStack.empty()){
		redoStack.pop();
	}

	// set the undo to active and redo to inactive
	undoAct->setEnabled(true);
	redoAct->setEnabled(false);

	refreshSaveIcon();

}




// run the current configuration
void MainWindow::run() {

	// validate configuration and show errors
	pair< vector<string>, vector<string> > errors = conf_.validate(mm_.getAllModuleMetaData());
	if (!errors.first.empty()) {
		LOG_E("There are configuration errors!");
		for(unsigned int i = 0; i < errors.first.size(); ++i) {
			LOG_E( errors.first[i]);
		}
		GUIEventDispatcher::instance()->triggerSelectNodesInGraphView(errors.second,gui::ERROR,false);
		return;
	}

	// stop is now activated and run unactivated
	stopAct->setEnabled(true);
	runAct->setEnabled(false);
	ui->progressBar->setHidden(false);
	ui->progressBar->update();

	if (workerThread_ != nullptr) return; //should not happen, because GUI prevents it. we only allow one chain to be processed by a thread

	workerThread_ = new RunWorkerThread(mm_,conf_);

	// Setup callback for cleanup when it finishes
	connect(workerThread_, SIGNAL(finished()),  this, SLOT(on_backgroundWorkerFinished()));

	// Run, Forest, run!
	workerThread_->start(); // This invokes WorkerThread::run in a new thread
}

//this gets called from Backgroundthread when its work is finished or when it gets terminated by stop()
void MainWindow::on_backgroundWorkerFinished()
{
	// run is now activated and stop unactivated
	stopAct->setEnabled(false);
	runAct->setEnabled(true);
	ui->progressBar->setHidden(true);
	ui->progressBar->update();
	delete workerThread_;
	workerThread_ = nullptr;
}

void MainWindow::stop() {

	if (workerThread_ == nullptr) return;

	//signal modules to stop
	workerThread_->stop();
	//give them some time
	workerThread_->wait(1000);
	//kill if not ready yet
	workerThread_->terminate();
	//not need to delete -> finished()

}

void MainWindow::keyReleaseEvent(QKeyEvent *)
{
	// resume a paused chain on key press
	mm_.resumeChain();
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
    connect(exitAct, SIGNAL(triggered()), this, SLOT(on_close()));

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
    runAct->setShortcut(QKeySequence(tr("Ctrl+R")));
    runAct->setStatusTip(tr("Run the configuration"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(run()));

    stopAct = new QAction(tr("&Stop"), this);
	stopAct->setShortcut(QKeySequence(tr("Shift+Ctrl+R")));
    stopAct->setStatusTip(tr("Stop the execution of the configuration"));
    stopAct->setEnabled(false); // initially inactive
    connect(stopAct, SIGNAL(triggered()), this, SLOT(stop()));

    closeWindowsAct = new QAction(tr("&Close windows"), this);
	closeWindowsAct->setShortcut(QKeySequence(tr("Ctrl+W")));
    closeWindowsAct->setStatusTip(tr("Close all open windows"));
    closeWindowsAct->setEnabled(true); // initially inactive
    connect(closeWindowsAct, SIGNAL(triggered()), this, SLOT(closeAllCreatedWindows()));
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
    delete closeWindowsAct;
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

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(closeWindowsAct);


    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

    //to make global keylisteners work Actions have to be added to the rootwidget (MainWindow.cpp) too
    addAction(newAct);
    addAction(openAct);
    addAction(saveAct);
    addAction(saveAsAct);
    addAction(exitAct);
    addAction(undoAct);
    addAction(redoAct);
    addAction(runAct);
    addAction(stopAct);
    addAction(closeWindowsAct);
    addAction(aboutAct);
}

