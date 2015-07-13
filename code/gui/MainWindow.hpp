#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include <QStandardItemModel>

#include <QDialog>
#include <QLabel>
#include <QStatusBar>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMessageBox>

#include <stack>

#include "models/ParamsModel.hpp"
#include "models/InputsDelegate.hpp"

#include "RunWorkerThread.h"

#include "../framework/Configuration.hpp"
#include "../framework/Logger.hpp"
#include "../framework/ProcessingStep.hpp"
#include "../framework/ModuleManager.hpp"
#include "../framework/GUIEventDispatcher.hpp"

#include "graph/graphwidget.h"
#include "graph/node.h"


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

	// loads a new configuration from file
	void loadDataFlow(std::string);

private slots:
	// Buttons addStep/deleteStep
    void on_addButton_clicked();
    void on_deleteButton_clicked();
	//stepName changed
	void on_stepNameChanged();

    // Activation of Step (via clicking)
    void on_stepSelectionChanged(const QItemSelection&);

    // append messages from our logger to the log-textview
    void on_appendToLog(const Logger::LogType&, const std::string& );
    // moves the progressbar on every step of the processing chain
    void on_reportProgress(const float& );

    //this gets called from Backgroundthread when its work is finished or when it gets terminated by stop()
    void on_backgroundWorkerFinished();

	// change of module dropdown
	void on_comboModule_currentIndexChanged(int);

	// change of module category dropdown
	void on_comboCategory_currentIndexChanged(int);

	// change in the params table
	void on_paramChanged(std::string, std::string);
	void on_inputChanged(std::string, std::pair<std::string, std::string>);

	void on_createWindow(const std::string& strTitle);

	void on_closeWindow(const std::string& strTitle);
	// menu bar
	// File
	void new_Data_Flow();
	void load_Data_Flow();
	void save_Data_Flow();
	void save_Data_Flow_as();
	void on_close();
	// Help
	void about();
	// Edit
	void undo();
	void redo();
	// Configuration
	void run();
	void stop();

	void closeAllCreatedWindows();

	void on_graphNodeSelected(const uipf::gui::Node* node);

	void on_clearLogButton_clicked();

	void on_logFiltertextChanged(const QString& text);

protected:
	void closeEvent(QCloseEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	// default window title that appears next to the file name
	const std::string WINDOW_TITLE = "uipf";

    Ui::MainWindow *ui;

	// the module manager instance
	ModuleManager mm_;

	// model for the listView of processing steps
    QStringListModel *modelStep;
	// model for the params editor table
    ParamsModel *modelTableParams;
	// models for the input editor table
    QStandardItemModel *modelTableInputs;
    // model delegate for the input editor table
	InputsDelegate *delegateTableInputs;

	// the file name of the currently loaded configuration
	std::string currentFileName;
	// asks the user, whether he wants to save the file
	bool okToContinue();
	// the currently loaded configuration represented in the window
   	Configuration conf_;

	// map of all available categories
	std::map<std::string, std::vector<std::string> > categories_;

	// counts the undo/redo, when = 0, it is the saved version
	int savedVersion = 1;
	// is true if file was at least one time saved
	bool unknownFile = true;

	// current name of a precessing step
	std::string currentStepName;

   	// Redo and Undo stacks, which store configurations
   	std::stack<Configuration> undoStack;
   	std::stack<Configuration> redoStack;
	// fills the undo and redo stacks
	void beforeConfigChange();

	// refresh UI triggers
	void refreshCategoryAndModule();
	void refreshParams();
	void refreshInputs();
	void refreshGraph();
	void refreshSaveIcon();

	// reset UI triggers
	void resetCategoryAndModule();
	void resetParams();
	void resetInputs();

	// menu bar
    void createActions();
    void deleteActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *configMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    // actions in fileMenu
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    // actions in editMenu
   	QAction *undoAct;
    QAction *redoAct;

    // actions in configMenu
   	QAction *runAct;
    QAction *stopAct;

    // actions in viewMenu
    QAction *closeWindowsAct;

    // actions in helpMenu
    QAction *aboutAct;

    //the view, that displays the graph
    gui::GraphWidget* graphView_;

    //our current backgroundworker or a nullptr
    RunWorkerThread* workerThread_;

    //keep track of all windows we created so we can close them later
    std::vector<QGraphicsView* > createdWindwows_;
};

} // namespace

#endif // MAINWINDOW_H
