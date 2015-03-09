#ifndef DAL_SQLITE_QUERIES_H_
#define DAL_SQLITE_QUERIES_H_

#include "db_indep.h"
#include "wrappers/sqlite_xx.h"

#include <memory>
#include <ostream>
#include <string>

namespace sqlite_queries {
class SQLiteTaskTableQueries : public storages::TaskTableQueries
{
public:
  explicit SQLiteTaskTableQueries(std::weak_ptr<sqlite3_cc::sqlite3> h);

private:
  virtual void createIfNotExistImpl();
  virtual void dropImpl() {

  }

  virtual void drawImpl(std::ostream& o) const {

  }

  std::string getTableName() const
  { return m_table_name; }

  const std::string m_table_name;
  std::weak_ptr<sqlite3_cc::sqlite3> m_conn_ptr;
};
}  // space

#endif
