#include <QApplication>
 
#include "mainWindow.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindowImpl *mw = new MainWindowImpl;
 
    mw->show();
    return app.exec();
}
