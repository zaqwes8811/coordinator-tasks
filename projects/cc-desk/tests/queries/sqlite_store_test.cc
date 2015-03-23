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

TEST(SQLite, TaskTable) {
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");
  auto table = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  table.registerBeanClass();
}

TEST(SQLite, TagAndTaskTables) {
  // Connect task with tag
  // select by tag

  using entities::Tag;
  using entities::TagEntity;

  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");

  auto tasks = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  tasks.registerBeanClass();

  auto tags = sqlite_queries::SQLiteTagTableQuery(h);
  tags.registerBeanClass();

  // Create tag
  // Must have unique name
  entities::Tag t(entities::EntityStates::kInactiveKey, "CUDA");
  tags.persist(t);
  entities::Tag t1(entities::EntityStates::kInactiveKey, "V8");
  tags.persist(t1);

  //storages::DataBase::dropSchema()
  tasks.drop();
  tags.drop();
}







