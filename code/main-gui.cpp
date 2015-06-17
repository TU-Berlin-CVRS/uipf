#include <QApplication>
#include <QTableView>

#include "framework/ModuleManager.hpp"
#include "gui/MainWindow.hpp"

using namespace std;
using namespace uipf;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	MainWindow w;

	// set the possible modules
	ModuleManager mm;
	QStringList list_modules;
	map<string, MetaData> modules = mm.getAllModuleMetaData();
	for (auto it = modules.begin(); it!=modules.end(); ++it) {
		list_modules << it->first.c_str();
	}
	w.setModuleList(list_modules);


    w.show();

    return a.exec();
}

