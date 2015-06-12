#include <QApplication>
#include <QTableView>

#include "framework/Configuration.hpp"
#include "gui/mainwindow.h"
#include "gui/processingstepsettings.h"

using namespace std;
using namespace uipf;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

// These steps should lated move to the File->Load Data Flow
   	Configuration conf;
    
	// loads the configFile and create a Configuration
	string configFileName = argv[1];
	conf.load(configFileName);
    
    // only for debug, print the loaded config
	conf.store("test.yaml");
// up to here

	QStringList list;
	
	map<string, ProcessingStep> chain = conf.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		list << it->first.c_str();
	}

	MainWindow w;
	
	// set the configuration steps
	w.setStepList(list);
	
    w.show();

    return a.exec();
}

