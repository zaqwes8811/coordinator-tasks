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

// цель - не включать заголовоки видов в модель - нужна виртуальность
// попробую статический полиморфизм.
template <typename View>
class ModelListener {
public:
  // Не владеет
  ModelListener(View* const view) : view_(view) { }

private:
  View* const view_;
};

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QWidget *window = new StartTest;

  ModelListener<QWidget> listener(window);

  window->show();

  // Пакуем вид и передаем модели
  // Patterns: Observer and Mediator

  return app.exec();
}
