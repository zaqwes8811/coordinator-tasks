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
using sqlite_queries::TagTableQuery;
using entities::EntityStates;


TEST(SQLite, TaskTable) {
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");
  auto table = sqlite_queries::TaskTableQueries(h, models::kTaskTableNameRef);
  table.registerBeanClass();
}

TEST(SQLite, TagAndTaskTables) {
  // Connect task with tag
  // select by tag
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");

  auto tasks = sqlite_queries::TaskTableQueries(h, models::kTaskTableNameRef);
  auto tags = sqlite_queries::TagTableQuery(h);
  tasks.registerBeanClass();
  tags.registerBeanClass();

  // Create tag
  tags.persist(Tag{EntityStates::kInactiveKey, "CUDA"});
  tags.persist(Tag{EntityStates::kInactiveKey, "V8"});

  {
  using renders::operator <<;
    std::cout << tags;
  std::cout << endl;
  cout << tasks;
  }

  // Create tasks

  // Destroy all
  tasks.drop();
  tags.drop();
}







