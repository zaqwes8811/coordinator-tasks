#include "heart/config.h"

#include "core/concepts.h"

#include <data_access_layer/postgresql_queries.h>
#include <data_access_layer/sqlite_queries.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <iostream>

using std::string;

// by value, not by type
enum db_vars { DB_SQLITE, DB_POSTGRES };

concepts::db_manager_concept_t build_database(const int selector) {
  if (true) { //selector == DB_SQLITE) {
    return concepts::db_manager_concept_t(sqlite_queries::SQLiteDataBase());
  } else {
    /*
    return concepts::db_manager_concept_t(
          pq_dal::PostgreSQLDataBase(models::kConnection, models::kTaskTableNameRef));
          */
  }
}

TEST(ConceptsTest, Test) {
  using namespace concepts;

  auto db = build_database(DB_SQLITE);
  auto tables = std::vector<table_concept_t>{db.getTaskTableQuery()
      //, db.getTaskTagQuery()
  };

  for (auto& a : tables)
    registerBeanClass(a);

  for (auto& a : tables)
    drop(a);
}

TEST(ConceptsTest, ActorEnv) {
  // FIXME: .then() how help if future is out of scope - see Sutter
  // FIXME: may be shared future?
  // FIXME: "push" and "pull" models
  // FIXME: need in another context, not in it actor - Actors and Wrappers!
  // Actor - Wrapper - Wrapper - Actor

  // FIXME: actor doubled interface!
  using namespace concepts;

  // trouble with handles in actor model
  // need real links
  // Object own it, but run in other thread
  auto db = build_database(DB_SQLITE);
  auto tables = std::vector<table_concept_t>{db.getTaskTableQuery()
//      , db.getTaskTagQuery()
};

  auto action = [tables] {
    for (auto& a : tables)
      registerBeanClass(a);
  };

  action();

  for (auto& a : tables)
    drop(a);
}
