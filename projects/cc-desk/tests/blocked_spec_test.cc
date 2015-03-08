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
#include <iostream>

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

class UIActor
{
public:

private:

};

// Actor model troubles:
//   https://www.qtdeveloperdays.com/2013/sites/default/files/presentation_pdf/Qt_Event_Loop.pdf
//   http://blog.bbv.ch/2012/10/03/multithreaded-programming-with-qt/
//
//   http://www.christeck.de/wp/2010/10/23/the-great-qthread-mess/
TEST(Blocked, TestApp) {
  int argc = 1;
  char* argv[1] = { "none" };
  QApplication app(argc, argv);

  storages::ConnectionPoolPtr pool(new pq_dal::PQConnectionsPool(models::kConnection));

  std::unique_ptr<models::Model> a(models::Model::createForOwn(pool));

  auto _ = MakeObjGuard(*a, &models::Model::clear_store);

  auto window = new Engine(a.get());

  shared_ptr<ModelListenerMediatorDynPolym> listener(new ModelListenerMediator(window));
  a->set_listener(listener);

  window->show();
  //app.exec();  // it's trouble for Actors usige

  // http://qt-project.org/doc/qt-4.8/qeventloop.html#processEvents
  while(true) {
    // ! can't sleep or wait!
    app.processEvents();  // hat processor!
  }
}

// FIXME: posting from other threads
//   http://qt-project.org/wiki/ThreadsEventsQObjects
