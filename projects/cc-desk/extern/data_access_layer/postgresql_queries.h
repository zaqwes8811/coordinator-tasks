#ifndef DAL_H
#define DAL_H

#include "model_layer/entities.h"

#include <pqxx/pqxx>

#include <string>
#include <vector>
#include <cassert>
#include <memory>

namespace pq_dal {

class TaskTableQueries
{
public:
  TaskTableQueries(const std::string& name, gc::WeakPtr<pqxx::connection> p)
    : m_table_name(name)
    , m_conn_ptr(p) { }

  void registerBeanClass();
  void drop();

private:
  const std::string m_table_name;
  gc::WeakPtr<pqxx::connection> m_conn_ptr;
  std::string getTableName() const
  { return m_table_name; }
};

class TaskLifetimeQueries
{
public:
  TaskLifetimeQueries(const std::string& table_name, gc::WeakPtr<pqxx::connection> p);

  // values op.
  entities::Task persist(const entities::Task& v);
  void update(const entities::Task& v);
  // entities op.
  entities::TaskEntities loadAll() const;

private:
  const std::string m_tableName;
  gc::WeakPtr<pqxx::connection> m_connPtr;
};

/**

*/
class PostgreSQLDataBase
    //: public storages::DataBase
{
public:
  PostgreSQLDataBase(const std::string& conn_info, const std::string& taskTableName);
  ~PostgreSQLDataBase();

  TaskTableQueries getTaskTableQuery();
  TaskLifetimeQueries getTaskLifetimeQuery();

private:
  // FIXME: important not only lifetime, but connection state to!
  gc::SharedPtr<pqxx::connection> m_conn_ptr;
  const std::string m_table_name;
};
}  // space


#endif
