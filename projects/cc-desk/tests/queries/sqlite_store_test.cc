#include "heart/config.h"

#include "sqlite_xx/sqlite_xx.h"
#include "data_access_layer/sqlite_queries.h"

#include <gtest/gtest.h>
#include <std_own_ext-fix/std_own_ext.h>

#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using entities::Tag;
using entities::TagEntity;

TEST(SQLite, TaskTable) {
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");
  auto table = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  table.registerBeanClass();
}

TEST(SQLite, TagAndTaskTables) {
  // Connect task with tag
  // select by tag
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");

  auto tasks = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  auto tags = sqlite_queries::SQLiteTagTableQuery(h);
  tasks.registerBeanClass();
  tags.registerBeanClass();

  // Create tag
  // Must have unique name
  entities::Tag t(entities::EntityStates::kInactiveKey, "CUDA");

  entities::Tag t1(entities::EntityStates::kInactiveKey, "V8");
  tags.persist(t);
  tags.persist(t1);

  //tags.lo

  //storages::DataBase::dropSchema()
  tasks.drop();
  tags.drop();
}







