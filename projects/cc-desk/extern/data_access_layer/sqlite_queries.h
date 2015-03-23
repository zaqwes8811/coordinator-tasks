#ifndef DAL_SQLITE_QUERIES_H_
#define DAL_SQLITE_QUERIES_H_

#include "model_layer/entities.h"

#include "sqlite_xx/sqlite_xx.h"

#include <memory>
#include <ostream>
#include <string>

namespace sqlite_queries {
class SQLiteTaskTableQueries
{
public:
  using Result = sqlite3_cc::Result;

  explicit SQLiteTaskTableQueries(gc::WeakPtr<sqlite3_cc::sqlite3> h
                                  , const std::string& tableName);

  /**
    \brief Persist new task.
  */
  entities::Task persist(const entities::Task& v);
  void update(const entities::Task& v);
  entities::TaskEntities loadAll() const;

  void registerBeanClass();
  void drop();

private:
  std::string getTableName() const
  { return m_tableName; }

  const std::string m_tableName;
  gc::WeakPtr<sqlite3_cc::sqlite3> m_connPtr;

  gc::SharedPtr<sqlite3_cc::sqlite3> lock() const {
    auto c = m_connPtr.lock();
    if (!c) throw std::runtime_error(FROM_HERE);
    return c;
  }

  Result exec(const std::string& sql) const {
    return sqlite3_exec(*lock(), sql);
  }
};

class SQLiteTagTableQuery
{
public:
  using Result = sqlite3_cc::Result;

  explicit SQLiteTagTableQuery(gc::WeakPtr<sqlite3_cc::sqlite3> h);
  void registerBeanClass();
  void drop();

  entities::Tag persist(const entities::Tag& tag);
private:
  const std::string m_tableName;
  gc::WeakPtr<sqlite3_cc::sqlite3> m_connPtr;

  gc::SharedPtr<sqlite3_cc::sqlite3> lock() const {
    auto c = m_connPtr.lock();
    if (!c) throw std::runtime_error(FROM_HERE);
    return c;
  }

  Result exec(const std::string& sql) const {
    return sqlite3_exec(*lock(), sql);
  }

  bool checkUnique(const std::string& name);
};

class SQLiteDataBase {
public:
  SQLiteDataBase()
    : m_connPtr(std::make_shared<sqlite3_cc::sqlite3>("test.db"))
      , m_taskTableName(models::kTaskTableNameRef)
  { }

  SQLiteTaskTableQueries  getTaskTableQuery()
  { return SQLiteTaskTableQueries(m_connPtr, m_taskTableName); }

  SQLiteTagTableQuery  getTagTableQuery()
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
