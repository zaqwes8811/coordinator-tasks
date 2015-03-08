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
  \attention Only in single thread! Actors Model can help

  http://herbsutter.com/2013/05/30/gotw-90-solution-factories/
*/
class ConnectionsPool : public boost::noncopyable {
public:
  explicit ConnectionsPool(const std::string& conn_info);
  ~ConnectionsPool();

  /**
    \fixme: strange design. May be bad lifetimes

    http://www.drdobbs.com/cpp/c11-uniqueptr/240002708
  */
  std::unique_ptr<storages::TaskTableQueries> createTaskTableQueries(const std::string& tablename);
  std::unique_ptr<storages::TaskLifetimeQueries> createTaskLifetimeQueries(const std::string& tablename);

private:
  // FIXME: important not only lifetime, but connection state to!
  boost::shared_ptr<pqxx::connection> m_conn_ptr;
};

typedef boost::shared_ptr<pq_dal::ConnectionsPool> PQConnectionPoolPtr;
}  // space


#endif
