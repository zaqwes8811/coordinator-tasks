// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:
#include "top/config.h"

#include "view/mainwindow.h"
#include "canary/model.h"
#include "dal/pq_queries.h"
#include "top/app_types.h"
#include "canary/isolation.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>

#include <boost/shared_ptr.hpp>
#include <loki/ScopeGuard.h>
#include <gtest/gtest.h>

#include <memory>
#include <cassert>

using namespace boost;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;

using ::isolation::ModelListenerMediatorDynPolym;

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


// Actor model troubles:
//   https://www.qtdeveloperdays.com/2013/sites/default/files/presentation_pdf/Qt_Event_Loop.pdf
//   http://blog.bbv.ch/2012/10/03/multithreaded-programming-with-qt/
TEST(Blocked, TestApp) {
  int argc = 1;
  char* argv[1] = { "none" };
  QApplication app(argc, argv);

  shared_ptr<pq_dal::ConnectionsPool> pool(new pq_dal::ConnectionsPool(models::kConnection));

  std::unique_ptr<models::Model> a(models::Model::createForOwn(pool));

  auto _ = MakeObjGuard(*a, &models::Model::clear_store);

  auto window = new Engine(a.get());

  shared_ptr<ModelListenerMediatorDynPolym> listener(new ModelListenerMediator(window));
  a->set_listener(listener);

  window->show();
  app.exec();
}

// FIXME: posting from other threads
//   http://qt-project.org/wiki/ThreadsEventsQObjects
