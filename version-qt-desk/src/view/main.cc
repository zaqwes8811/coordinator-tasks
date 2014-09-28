// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:
#include "top/config.h"

// App
#include "view/mainwindow.h"
#include "canary/app_core.h"
#include "canary/pq_queries.h"
#include "canary/app_types.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>

#include <boost/shared_ptr.hpp>

#include <memory>
#include <cassert>

using namespace boost;

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
  // Model

  // View
  QApplication app(argc, argv);

  shared_ptr<pq_dal::PQConnectionPool> pool(new pq_dal::PQConnectionPool(app_core::kConnection));
  std::auto_ptr<app_core::AppCore> a(app_core::AppCore::createInHeap(pool));

  QWidget *window = new StartTest(a.get());

  //ModelListenerStaticPolym<QWidget> listener(window);
  ModelListenerMediator listener(window);

  window->show();

  // Пакуем вид и передаем модели
  // Patterns: Observer and Mediator

  return app.exec();
}
