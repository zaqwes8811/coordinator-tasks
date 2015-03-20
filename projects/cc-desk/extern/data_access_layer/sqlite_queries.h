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

  entities::Task persist(const entities::Task& v) {
    return v;
  }
  void update(const entities::Task& v) {}

  entities::TaskEntities loadAll() const {
    return entities::TaskEntities();
  }

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

class SQLiteDataBase {
public:
  SQLiteDataBase()
    : m_connPtr(std::make_shared<sqlite3_cc::sqlite3>("test.db"))
      , m_taskTableName(models::kTaskTableNameRef)
  { }

  SQLiteTaskTableQueries  getTaskTableQuery()
  { return SQLiteTaskTableQueries(m_connPtr, m_taskTableName); }

  SQLiteTagTableQuery  getTaskTagQuery()
  { return SQLiteTagTableQuery(m_connPtr); }

  SQLiteTaskTableQueries getTaskLifetimeQuery()
  { return SQLiteTaskTableQueries(m_connPtr, m_taskTableName); }

private:
  // FIXME: important not only lifetime, but connection state to!
  gc::SharedPtr<sqlite3_cc::sqlite3> m_connPtr;
  const std::string m_taskTableName;
};

}  // space

#endif
