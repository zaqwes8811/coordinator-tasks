#include "top/config.h"

#include "sqlite_queries.h"

namespace sqlite_queries {
SQLiteTaskTableQueries::SQLiteTaskTableQueries(app::WeakPtr<sqlite3_cc::sqlite3> h)
  : m_table_name("h")
  , m_conn_ptr(h) { }

void SQLiteTaskTableQueries::createIfNotExistImpl() {
  std::string sql(
    "CREATE TABLE " \
    "IF NOT EXISTS "+
    m_table_name +
    "(" \
    "ID         SERIAL PRIMARY KEY NOT NULL," \
    "TASK_NAME  TEXT               NOT NULL, " \
    "PRIORITY   INT                NOT NULL, " \
    "DONE BOOLEAN DEFAULT FALSE);");

  auto c = m_conn_ptr.lock();

  if (!c)
    return;

  sqlite3_cc::sqlite3_exec(*c, sql);
}
}  // space
