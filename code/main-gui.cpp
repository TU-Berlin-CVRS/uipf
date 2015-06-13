#include <QApplication>
#include <QTableView>

#include "framework/Configuration.hpp"
#include "framework/ModuleManager.hpp"
#include "gui/mainwindow.h"
#include "gui/processingstepsettings.h"

using namespace std;
using namespace uipf;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

// These steps should later move to the File->Load Data Flow
   	Configuration conf;

	// loads the configFile and create a Configuration
	string configFileName = argv[1];
	conf.load(configFileName);

    // only for debug, print the loaded config
	conf.print();
// up to here

	MainWindow w;

	// set the names of the processing steps:
	QStringList list;
	map<string, ProcessingStep> chain = conf.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		list << it->first.c_str();
	}
	w.setStepList(list);
	
	// set the possible models
	ModuleManager mm;
	QStringList list_modules;
	map<string, MetaData> modules = mm.getAllModuleMetaData();
	for (auto it = modules.begin(); it!=modules.end(); ++it) {
		list_modules << it->first.c_str();
	}
	w.setModuleList(list_modules);
	

	// the following line is only for testing, this step should be done by clicking on a processing step
	ProcessingStep prst = chain["process"]; 
	w.setStepParams(prst);

	
    w.show();

    return a.exec();
}

