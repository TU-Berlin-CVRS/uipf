#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QListWidgetItem>

#include <QDialog>
#include <QLabel>
#include <QStatusBar>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <stack>

#include "ProcessingStepParams.hpp"
#include "ProcessingStepInputs.hpp"

#include "../framework/Configuration.hpp"
#include "../framework/Logger.hpp"
#include "../framework/ProcessingStep.hpp"

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
	// Buttons addStep/deleteStep
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    
    // Activation of Step (via clicking)
    void on_listProcessingSteps_activated(const QModelIndex & index);
    void on_appendToLog(const Logger::LogType&, const std::string& );

	// menu bar
	// File
	void new_Data_Flow();
	void load_Data_Flow();
	void save_Data_Flow();
	void save_Data_Flow_as();
	// Help
	void about();
	// Edit
	void undo();
	void redo();
	// Configuration
	void run();
	void stop();
	
	//stepName changed
	void stepNameChanged();
	
private:
    Ui::MainWindow *ui;

	// all the configuration steps
    QStringListModel *modelStep;

	// all the modules
    QStringListModel *modelModule;
	
	// current configuration
	std::string currentFileName;

    ProcessingStepParams *modelTableParams;
    ProcessingStepInputs *modelTableInputs;

	// the currently loaded configuration represented in the window
   	Configuration conf_;
   	// Redo and Undo stacks, which store configurations
   	std::stack<Configuration> undoStack;  
   	std::stack<Configuration> redoStack;  
	// fills the stacks
	void configChanged();
   	
	// menu bar
    void createActions();
    void createMenus();
    
    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *editMenu;
    QMenu *configMenu;
    
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    
    QAction *aboutAct;
    //~ QLabel *infoLabel;
   	
   	QAction *undoAct;
    QAction *redoAct;
   	
   	QAction *runAct;
    QAction *stopAct;
   	
};

}; // namespace

#endif // MAINWINDOW_H
