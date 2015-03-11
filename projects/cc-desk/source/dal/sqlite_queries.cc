#include "top/config.h"

#include "sqlite_queries.h"

namespace sqlite_queries {
SQLiteTaskTableQueries::SQLiteTaskTableQueries(app::WeakPtr<sqlite3_cc::sqlite3> h
                                               , const std::string& tableName)
  : m_tableName(tableName)
  , m_connPtr(h) { }

void SQLiteTaskTableQueries::createIfNotExistImpl() {
  std::string sql(
    "CREATE TABLE " \
    "IF NOT EXISTS "+
    m_tableName +
    "(" \
    "ID         SERIAL PRIMARY KEY NOT NULL," \
    "TASK_NAME  TEXT               NOT NULL, " \
    "PRIORITY   INT                NOT NULL, " \
    "DONE BOOLEAN DEFAULT FALSE);");

  auto c = m_connPtr.lock();

  if (!c)
    return;

  sqlite3_cc::sqlite3_exec(*c, sql);
}

void SQLiteTaskTableQueries::dropImpl() {
  auto c = m_connPtr.lock();

  if (!c)
    return;

  sqlite3_cc::sqlite3_exec(*c, "DROP TABLE " + m_tableName + ";");
}

void SQLiteTaskTableQueries::drawImpl(std::ostream& o) const {

}
}  // space
