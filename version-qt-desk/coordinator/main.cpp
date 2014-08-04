// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:

#include <QApplication>
#include <QLabel>

// App
#include "mainwindow.h"

// How add widget
// http://stackoverflow.com/questions/17989231/how-to-add-buttons-to-a-main-window-in-qt
//
// Signals and slots:
// http://qt-project.org/wiki/Qt_for_beginners_Signals_and_slots_2

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    QLabel *label = new QLabel("Hello Qt!");
    label->show();


    // Пакуем вид и передаем модели
    // Patterns: Observer and Mediator
    
    return a.exec();
}

/*

int main(int argc, char** argv)
{
 QApplication app(argc, argv);
 QWidget wid;
 QPushButton *button = new QPushButton(&wid);
 button->setMinimumWidth(68);
 button->setMinimumHeight(60);
 QVBoxLayout *layout = new QVBoxLayout(&wid);
 layout->addWidget(button);
 wid.show();
 return app.exec();
}
*/
