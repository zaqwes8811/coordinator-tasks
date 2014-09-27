// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>

// App
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);


  QWidget *window = new StartTest;
  window->show();

  // Пакуем вид и передаем модели
  // Patterns: Observer and Mediator

  return app.exec();
}
