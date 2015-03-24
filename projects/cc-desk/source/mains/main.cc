#include "heart/config.h"

#include "view/mainwindow.h"
#include "model_layer/model.h"
#include "data_access_layer/postgresql_queries.h"
#include "common/app_types.h"
#include "model_layer/isolation.h"
#include "core/actor_ui.h"
#include "core/scopes.h"
#include "core/concepts.h"

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTableWidget>
#include <loki/ScopeGuard.h>
#include <data_access_layer/sqlite_queries.h>
#include <gtest/gtest.h>
#include <actors_and_workers/concurent_queues.h>

#include <memory>
#include <cassert>
#include <iostream>

using Loki::ScopeGuard;
using Loki::MakeObjGuard;

using isolation::ModelListener;

namespace {
// by value, not by type
enum db_vars { DB_SQLITE, DB_POSTGRES };

concepts::db_manager_concept_t build_database(const int selector) {
  if (true) { //selector == DB_SQLITE) {
    return concepts::db_manager_concept_t(sqlite_queries::SQLiteDataBase());
  } else {
    /*return
        concepts::db_manager_concept_t(pq_dal::PostgreSQLDataBase(
                                                models::kConnection, models::kTaskTableNameRef
                                                ));*/
  }
}
}

//static
auto gUIActor = std::make_shared<actors::UIActor>();  // dtor will call and app out
//static
auto gDBActor = std::make_shared<cc11::Actior>();

int main() {
  scopes::AppScope app;

  // FIXME: put in actor?
  auto db = build_database(DB_SQLITE);
  auto f = gUIActor->connectUI(db);

  return f.get();
}



