#ifndef DAL_H
#define DAL_H

#include "model_layer/entities.h"
#include "db_indep.h"

#include <pqxx/pqxx>

#include <string>
#include <vector>
#include <cassert>
#include <memory>

namespace pq_dal {

class TaskTableQueries
    : public storages::TaskTableQueries
{
public:
  TaskTableQueries(const std::string& name, app::WeakPtr<pqxx::connection> p)
    : m_table_name(name)
    , m_conn_ptr(p) { }

private:
  const std::string m_table_name;
  app::WeakPtr<pqxx::connection> m_conn_ptr;

  void do_registerBeanClass() override;

  void do_drop() override;

  std::string getTableName() const override
  { return m_table_name; }
};

class TaskLifetimeQueries
    : public storages::TaskLifetimeQueries
{
public:
  TaskLifetimeQueries(const std::string& table_name
                              , app::WeakPtr<pqxx::connection> p);
private:
  const std::string m_tableName;
  app::WeakPtr<pqxx::connection> m_connPtr;

  // values op.
  entities::Task do_create(const entities::Task& v) override;

  void do_update(const entities::Task& v) override;

  // entities op.
  entities::TaskEntities do_loadAll() const override;
};

/**

*/
class PQConnectionsPool : public storages::DataBaseDriver {
public:
  PQConnectionsPool(const std::string& conn_info, const std::string& table_name);
  ~PQConnectionsPool();

  std::unique_ptr<storages::TaskTableQueries> createTaskTableQuery();
  std::unique_ptr<storages::TaskLifetimeQueries> createTaskLifetimeQuery();

private:
  // FIXME: important not only lifetime, but connection state to!
  app::SharedPtr<pqxx::connection> m_conn_ptr;
  const std::string m_table_name;
};
}  // space


#endif
