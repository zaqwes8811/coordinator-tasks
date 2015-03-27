#ifndef DAL_SQLITE_QUERIES_H_
#define DAL_SQLITE_QUERIES_H_

#include "model_layer/entities.h"

#include "sqlite_xx/sqlite_xx.h"

#include <memory>
#include <ostream>
#include <string>
#include <iostream>

namespace renders {
template <typename T>
std::ostream& operator<< (std::ostream &o, T &t) {
  using sqlite3_cc::operator<<;

  std::string sql = "select * from " + t.m_table_name + ";";
  auto r = t.exec(sql);

  o << r;

  return o;
}
}

namespace sqlite_queries {



class TaskTableQueries
{
public:
  using Result = sqlite3_cc::Result;

  explicit TaskTableQueries(gc::WeakPtr<sqlite3_cc::sqlite3> h
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
  template <typename T>
  friend std::ostream& renders::operator<< (std::ostream &o, T &t);

  std::string getTableName() const
  { return m_table_name; }

  const std::string m_table_name;
  gc::WeakPtr<sqlite3_cc::sqlite3> m_connPtr;

  gc::SharedPtr<sqlite3_cc::sqlite3> lock() const {
    auto c = m_connPtr.lock();
    if (!c) throw std::runtime_error(FROM_HERE);
    return c;
  }

  // FIXME: Move to store. For monitoring SQL requests
  Result exec(const std::string& sql) const;
};

class TagTableQuery
{
public:
  using Result = sqlite3_cc::Result;

  explicit TagTableQuery(gc::WeakPtr<sqlite3_cc::sqlite3> h);
  void registerBeanClass();
  void drop();

  entities::Tag persist(const entities::Tag& tag);

private:
  template <typename T>
  friend std::ostream& renders::operator<< (std::ostream &o, T &t);

  const std::string m_table_name;
  gc::WeakPtr<sqlite3_cc::sqlite3> m_connPtr;

  gc::SharedPtr<sqlite3_cc::sqlite3> lock() const {
    auto c = m_connPtr.lock();
    if (!c) throw std::runtime_error(FROM_HERE);
    return c;
  }

  Result exec(const std::string& sql) const {
    // ! trouble !
    //std::cout << sql << std::endl;
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

  TaskTableQueries  getTaskTableQuery()
  { return TaskTableQueries(m_connPtr, m_taskTableName); }

  TagTableQuery  getTagTableQuery()
  { return TagTableQuery(m_connPtr); }

  TaskTableQueries getTaskLifetimeQuery()
  { return TaskTableQueries(m_connPtr, m_taskTableName); }

private:
  // FIXME: important not only lifetime, but connection state to!
  gc::SharedPtr<sqlite3_cc::sqlite3> m_connPtr;
  const std::string m_taskTableName;
};

}  // space

#endif
