// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:

#include <QApplication>

// App
#include "mainwindow.h"

// How add widget
// http://stackoverflow.com/questions/17989231/how-to-add-buttons-to-a-main-window-in-qt
//

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
