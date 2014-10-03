// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:
#include "top/config.h"

#include "view/mainwindow.h"
#include "canary/model.h"
#include "canary/pq_queries.h"
#include "top/app_types.h"
#include "canary/isolation.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>
#include <boost/shared_ptr.hpp>
#include <loki/ScopeGuard.h>

#include <memory>
#include <cassert>

using namespace boost;
using ::isolation::ModelListenerMediatorDynPolym;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;

class ModelListenerMediator : public ModelListenerMediatorDynPolym {
public:
  explicit ModelListenerMediator(Engine* const view) : view_(view) {
    // не должно быть нулем
  }

private:
  void update_() {
    assert(view_);
    view_->redraw();
  }

  Engine* const view_;
};

int main(int argc, char *argv[])
{
  // Model

  // View
  QApplication app(argc, argv);

  shared_ptr<pq_dal::PQConnectionPool> pool(new pq_dal::PQConnectionPool(models::kConnection));
  //shared_ptr
  std::auto_ptr<models::Model> a(models::Model::createInHeap(pool));

  // Пока очищаем хранилище
  ScopeGuard _ = MakeObjGuard(*a, &models::Model::clear_store);

  Engine *window = new Engine(a.get());

  //ModelListenerStaticPolym<QWidget> listener(window);
  shared_ptr<ModelListenerMediatorDynPolym> listener(new ModelListenerMediator(window));
  a->set_listener(listener);

  window->show();

  // Пакуем вид и передаем модели
  // Patterns: Observer and Mediator

  // FIXME: run event loop?
  return app.exec();
}
