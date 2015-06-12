#include <QApplication>
#include <QTableView>


#include "gui/mainwindow.h"
#include "gui/processingstepsettings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

