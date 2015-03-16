#include "heart/config.h"

#include "wrappers/sqlite_xx.h"
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

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
TEST(SQLiteTest, Base) {
  sqlite3_cc::sqlite3 h("test.db");


  /* Create SQL statement */
  string sql = "CREATE TABLE IF NOT EXISTS COMPANY("  \
           "ID INT PRIMARY KEY     NOT NULL," \
           "NAME           TEXT    NOT NULL," \
           "AGE            INT     NOT NULL," \
           "ADDRESS        CHAR(50)," \
           "SALARY         REAL );";

   /* Execute SQL statement */
  sqlite3_cc::sqlite3_exec(h, sql);

  /* Create SQL statement */
  sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
       "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
       "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
       "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
       "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
       "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
       "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
       "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

  /* Execute SQL statement */
  sqlite3_cc::sqlite3_exec(h, sql);

  /* Create SQL statement */
  sql = "SELECT * from COMPANY";

  /* Execute SQL statement */
  const auto result = sqlite3_cc::sqlite3_exec(h, sql);
  std::cout << result;


  sqlite3_cc::sqlite3_exec(h, "drop table COMPANY;");
}

static const string s_kTagTableName = "TAGS";

class SQLiteTagTableQuery
{
public:
  explicit SQLiteTagTableQuery(app::WeakPtr<sqlite3_cc::sqlite3> h)
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
  app::WeakPtr<sqlite3_cc::sqlite3> m_connPtr;
};

TEST(SQLite, TaskTable) {
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");
  auto table = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  table.createIfNotExist();
}

bool checkUnique(const std::string& name, app::WeakPtr<sqlite3_cc::sqlite3> h) {
  auto c = h.lock();
  if (!c) return false;

  auto sql = "SELECT NAME FROM " + s_kTagTableName + " WHERE NAME ='" + name + "';";
  auto r = sqlite3_cc::sqlite3_exec(*c, sql);
  return r.empty();
}

app::SharedPtr<entities::TagEntity>
createTag(const values::Tag& tag, app::WeakPtr<sqlite3_cc::sqlite3> h) {
  DCHECK(tag.m_primaryKey == entities::EntityStates::kInactiveKey);
  DCHECK(checkUnique(tag.m_name, h));

  // http://stackoverflow.com/questions/531109/how-to-return-the-value-of-auto-increment-column-in-sqlite-with-vb6
  string sql(
      "INSERT INTO " + s_kTagTableName + " (NAME, COLOR) " \
      "VALUES ('" + tag.m_name+"','" + tag.m_color + "'); " \
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
    id = sqlite3_cc::as<size_t>(col["ID"]);
    break;
  }
  DCHECK(id != entities::EntityStates::kInactiveKey);

  auto entity = std::make_shared<entities::TagEntity>();

  return entity;
}

TEST(SQLite, TagAndTaskTables) {
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");

  auto tasks = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  tasks.createIfNotExist();

  auto tags = SQLiteTagTableQuery(h);
  tags.createIfNotExist();

  // Create tag
  // Must have unique name
  values::Tag t(entities::EntityStates::kInactiveKey, "CUDA");
  createTag(t, h);

  tasks.drop();
  tags.drop();
}







