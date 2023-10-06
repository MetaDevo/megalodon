#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationVersion("0.0.0");
    QCoreApplication::setApplicationName("Megalodon");
    QCoreApplication::setOrganizationName("MetaDevo");
    QCoreApplication::setOrganizationDomain("metadevo.com");

    QApplication a(argc, argv);
    a.setStyle("fusion"); // should automatically change to dark mode per system context
    MainWindow w;
    w.show();
    return a.exec();
}
