// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:

// App
#include "view/mainwindow.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>

#include <cassert>

// цель - не включать заголовоки видов в модель - нужна виртуальность
// попробую статический полиморфизм.
template <typename View>  // тогда и модель становится шаблоном, а значит код выносится в заголовки
class ModelListenerStaticPolym {
public:
  // Не владеет
  ModelListenerStaticPolym(View* const view) : view_(view) { }

  void update() {
    //view_->
  }

private:
  View* const view_;
};

class ModelListenerMediatorDynPolym {
public:
  virtual ~ModelListenerMediatorDynPolym() { }
  void update() {
    //
    update_();
  }
private:
  virtual void update_() = 0;
};

class ModelListenerMediator : public ModelListenerMediatorDynPolym {
public:
  explicit ModelListenerMediator(QWidget* const view) : view_(view) {
    // не должно быть нулем
  }

private:
  void update_() {
    assert(view_);
  }

  QWidget* const view_;
};

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QWidget *window = new StartTest;

  //ModelListenerStaticPolym<QWidget> listener(window);
  ModelListenerMediator listener(window);

  window->show();

  // Пакуем вид и передаем модели
  // Patterns: Observer and Mediator

  return app.exec();
}
