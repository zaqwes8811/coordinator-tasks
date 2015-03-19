// UI:
// http://qt-project.org/doc/qt-4.8/qtablewidget.html#details
//
// Model:
//
// Store:
#include "heart/config.h"

#include "view/mainwindow.h"
#include "model_layer/model.h"
#include "data_access_layer/postgresql_queries.h"
#include "common/app_types.h"
#include "model_layer/isolation.h"
#include "core/actor_ui.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>
#include <loki/ScopeGuard.h>
#include <gtest/gtest.h>
#include <actors_and_workers/concurent_queues.h>

#include <memory>
#include <cassert>
#include <iostream>
#include <thread>

using Loki::ScopeGuard;
using Loki::MakeObjGuard;

using isolation::ModelListener_virtual;

class ModelListenerMediator : public ModelListener_virtual {
public:
  explicit ModelListenerMediator(UiEngine* const view) : view_(view) { }

private:
  void do_update() {
    DCHECK(view_);
    view_->redraw();
  }

  UiEngine* const view_;
};

TEST(Blocked, UIActorTest) {
  // work in DB thread
  storages::DataBasePtr pool(
        new pq_dal::PostgreSQLDataBase(models::kConnection, models::kTaskTableNameRef));


  // work in UI thread
  auto modelPtr = gc::SharedPtr<models::Model>(models::Model::createForOwn(pool));

  auto ui = std::make_shared<actors::UIActor>(modelPtr);  // dtor will call and app out

  ui->post([modelPtr] {
    modelPtr->getCurrentModelData();
  });

  // FIXME: troubles with out appl.
  while(true) { std::this_thread::sleep_for(std::chrono::milliseconds(50)); }  // bad!
}

// FIXME: Boost.Signal



