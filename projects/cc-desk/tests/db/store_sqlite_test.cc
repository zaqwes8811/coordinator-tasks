#include "heart/config.h"

#include "sqlite_xx/sqlite_xx.h"
#include "data_access_layer/db_indep.h"
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

std::ostream& operator<<(std::ostream& o, const sqlite3_cc::Result& result) {
  for(auto& row: result) {
    for(auto& column: row) {
      o << column.first << " = " << column.second << " ";
    }
    o  << std::endl;
  }
  return o;
}

static const string s_kTagTableName = "TAGS";

class SQLiteTagTableQuery
{
public:
  explicit SQLiteTagTableQuery(gc::WeakPtr<sqlite3_cc::sqlite3> h)
    : m_tableName(s_kTagTableName)
    , m_connPtr(h) { }

  void createIfNotExist()  {
    // http://www.tutorialspoint.com/sqlite/sqlite_using_autoincrement.htm
    string sql = "CREATE TABLE IF NOT EXISTS " + m_tableName + "("  \
             "ID             INTEGER  PRIMARY KEY     AUTOINCREMENT," \
             "NAME           TEXT                    NOT NULL," \
             "COLOR          TEXT                    NOT NULL);";

    auto c = m_connPtr.lock();
    if (!c) return;
    sqlite3_cc::sqlite3_exec(*c, sql);
  }

  void drop() {
    auto c = m_connPtr.lock();
    if (!c) return;
    sqlite3_cc::sqlite3_exec(*c, "DROP TABLE " + m_tableName + ";");
  }

private:
  const std::string m_tableName;
  gc::WeakPtr<sqlite3_cc::sqlite3> m_connPtr;
};

TEST(SQLite, TaskTable) {
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");
  auto table = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  table.registerBeanClass();
}

bool checkUnique(const std::string& name, gc::WeakPtr<sqlite3_cc::sqlite3> h) {
  auto c = h.lock();
  if (!c) return false;

  auto sql = "SELECT NAME FROM " + s_kTagTableName + " WHERE NAME ='" + name + "';";
  auto r = sqlite3_cc::sqlite3_exec(*c, sql);
  return r.empty();
}

entities::TagEntity createTag(const entities::Tag& tag, gc::WeakPtr<sqlite3_cc::sqlite3> h) {
  using sqlite3_cc::as;

  DCHECK(tag.id == entities::EntityStates::kInactiveKey);
  DCHECK(checkUnique(tag.name, h));

  // http://stackoverflow.com/questions/531109/how-to-return-the-value-of-auto-increment-column-in-sqlite-with-vb6
  string sql(
      "INSERT INTO " + s_kTagTableName + " (NAME, COLOR) " \
      "VALUES ('" + tag.name+"','" + tag.color + "'); " \
      "  SELECT last_insert_rowid() FROM " + s_kTagTableName + "; ");

  auto c = h.lock();
  if (!c)
    throw std::runtime_error(FROM_HERE);

  // query
  auto r = sqlite3_cc::sqlite3_exec(*c, sql);
  DCHECK(r.size() == 1);

  // Узнаем что за ключ получили
  size_t id(entities::EntityStates::kInactiveKey);
  for (auto& col : r) {
    id = as<size_t>(col["ID"]);
    break;
  }
  DCHECK(id != entities::EntityStates::kInactiveKey);

  auto entity = std::make_shared<entities::Tag>();
  entity->id = id;
  entity->name = tag.name;
  entity->color = tag.color;

  return entity;
}

template <typename Table>
void registerTable (Table& t) { }

TEST(SQLite, TagAndTaskTables) {
  // Connect task with tag
  // select by tag

  using entities::Tag;
  using entities::TagEntity;

  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");

  auto tasks = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  tasks.registerBeanClass();

  auto tags = SQLiteTagTableQuery(h);
  tags.createIfNotExist();

  // Create tag
  // Must have unique name
  entities::Tag t(entities::EntityStates::kInactiveKey, "CUDA");
  auto e = createTag(t, h);

  //storages::DataBase::dropSchema()
  tasks.drop();
  tags.drop();
}







