#include <QApplication>

#include "framework/ModuleManager.hpp"
#include "gui/MainWindow.hpp"

using namespace std;
using namespace uipf;

int main(int argc, char *argv[])
{
	// create Qt Application
	QApplication a(argc, argv);

	// show the main window
	MainWindow w;
	w.show();

	// it is possible to give a config file as first argument
	if (argc > 1) {
		w.loadDataFlow(argv[1]);
	}

	return a.exec();
}
