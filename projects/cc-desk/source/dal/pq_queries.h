#ifndef DAL_H
#define DAL_H

#include "canary/entities_and_values.h"
#include "db_indep.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
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
  TaskTableQueries(const std::string& name, boost::weak_ptr<pqxx::connection> p)
    : m_table_name(name)
    , m_conn_ptr(p) { }

private:
  const std::string m_table_name;
  boost::weak_ptr<pqxx::connection> m_conn_ptr;

  void createIfNotExistImpl();
  void dropImpl();

  std::string getTableName() const
  { return m_table_name; }

  void drawImpl(std::ostream& o) const;
};

class TaskLifetimeQueries
    : public storages::TaskLifetimeQueries
{
public:
  TaskLifetimeQueries(const std::string& table_name
                              , boost::weak_ptr<pqxx::connection> p);
private:
  const std::string m_table_name;
  boost::weak_ptr<pqxx::connection> m_conn_ptr;

  // values op.
  values::ImmutableTask createImpl(const values::ImmutableTask& v);

  void updateImpl(const values::ImmutableTask& v);

  // entities op.
  entities::Tasks get_allImpl() const;
};

/**

*/
class PQConnectionsPool : public storages::DataBaseDriver {
public:
  PQConnectionsPool(const std::string& conn_info, const std::string& table_name);
  ~PQConnectionsPool();

  std::unique_ptr<storages::TaskTableQueries> createTaskTableQueries();
  std::unique_ptr<storages::TaskLifetimeQueries> createTaskLifetimeQueries();

private:
  // FIXME: important not only lifetime, but connection state to!
  boost::shared_ptr<pqxx::connection> m_conn_ptr;
  const std::string m_table_name;
};
}  // space


#endif
