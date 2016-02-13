#include "heart/config.h"

#include "view/mainwindow.h"
#include "model_layer/model.h"
#include "data_access_layer/postgresql_queries.h"
#include "common/app_types.h"
#include "model_layer/isolation.h"
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
#include <actors_and_workers/actor_ui.h>
#include <actors_and_workers/arch.h>

#include <memory>
#include <cassert>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdio.h>

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

// http://stackoverflow.com/questions/4025370/can-an-executable-discover-its-own-path-linux
int main(int argc, char** argv) {
  //for (int i = 0; i < argc; ++i)
  //  std::cout << argv[i] << std::endl;

  char dest[PATH_MAX];
  char path[PATH_MAX];
  sprintf(path, "/proc/self/exe");
  DCHECK(readlink(path, dest, PATH_MAX) != -1);

  auto bin_name = std::string(dest);//std::string(argv[0]);
  auto found = bin_name.find_last_of("/");
  auto work_dir = bin_name.substr(0, found);

  system(std::string("cd " + work_dir).c_str());
  DCHECK(chdir(work_dir.c_str()) != -1);

  //std::cout << work_dir << std::endl;
  //std::cout << get_current_dir_name() << std::endl;

  auto db = build_database(DB_SQLITE);
  auto f = Workers::ActivateUiEventLoop(db);
  return f.get();
}



