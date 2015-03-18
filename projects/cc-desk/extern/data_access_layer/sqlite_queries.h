#ifndef DAL_SQLITE_QUERIES_H_
#define DAL_SQLITE_QUERIES_H_

#include "db_indep.h"
#include "sqlite_xx/sqlite_xx.h"

#include <memory>
#include <ostream>
#include <string>

namespace sqlite_queries {
class SQLiteTaskTableQueries : public storages::TaskTableQueries
{
public:
  explicit SQLiteTaskTableQueries(gc::WeakPtr<sqlite3_cc::sqlite3> h
                                  , const std::string& tableName);

private:
  virtual void do_registerBeanClass();
  virtual void do_drop();
  virtual void drawImpl(std::ostream& o) const;

  std::string getTableName() const
  { return m_tableName; }

  const std::string m_tableName;
  gc::WeakPtr<sqlite3_cc::sqlite3> m_connPtr;
};

class SQLiteTagTableQuery
{
public:
  explicit SQLiteTagTableQuery(gc::WeakPtr<sqlite3_cc::sqlite3> h);
  void registerBeanClass();
  void drop();
private:
  const std::string m_tableName;
  gc::WeakPtr<sqlite3_cc::sqlite3> m_connPtr;
};

bool checkUnique(const std::string& name, gc::WeakPtr<sqlite3_cc::sqlite3> h);
entities::TagEntity createTag(const entities::Tag& tag, gc::WeakPtr<sqlite3_cc::sqlite3> h);

}  // space

#endif
