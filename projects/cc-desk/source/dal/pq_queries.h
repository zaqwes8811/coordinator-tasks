#ifndef DAL_H
#define DAL_H

#include "canary/entities_and_values.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <pqxx/pqxx>

#include <string>
#include <vector>
#include <cassert>

namespace pq_dal {
class ConnectionsPool;

/**
  \attention Small life time!
*/
class TaskTableQueries
    //: public boost::noncopyable
{
public:
  void createIfNotExist();
  void drop();

  void draw(std::ostream& o) const;

private:
  friend class ConnectionsPool;

public:
  TaskTableQueries(const std::string& name, boost::weak_ptr<pqxx::connection> p)
    : m_table_name(name)
    , m_conn_ptr(p) { }

private:
  const std::string m_table_name;
  boost::weak_ptr<pqxx::connection> m_conn_ptr;
};

/**
  \attention Small life time!

  Делать один репозиторий не советуют
  TODO: Может DI какой сделать, или все равно?
  FIXME: как то объединить create, update, etc. в persist

  Назначет id!! очень важно! объекты уникальные
  Создает, если еще не был создан, либо обновляет всю запись
  by value
  На групповую вставку могут быть ограничения, но в данной задаче
   пока не нужно, если не нужно будет что-то куда-то автоматически переливать.

  FIXME: с умными указателями возникают проблемы с константростью!
*/
class TaskLifetimeQueries
    //: public boost::noncopyable
{
public:
  // values op.
  values::ImmutableTask create(const values::ImmutableTask& v);

  void update(const values::ImmutableTask& v);

  // entities op.
  entities::Tasks get_all() const;

private:
  friend class ConnectionsPool;
public:
  TaskLifetimeQueries(const std::string& table_name
                              , boost::weak_ptr<pqxx::connection> p);
private:
  const std::string m_table_name;
  boost::weak_ptr<pqxx::connection> m_conn_ptr;
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
  */
  TaskTableQueries createTaskTableQueries(const std::string& tablename) {
    return TaskTableQueries(tablename, m_conn_ptr);
  }

  TaskLifetimeQueries createTaskLifetimeQueries(const std::string& tablename) {
    return TaskLifetimeQueries(tablename, m_conn_ptr);
  }

private:
  // FIXME: important not only lifetime, but connection state to!
  boost::shared_ptr<pqxx::connection> m_conn_ptr;
};

typedef boost::shared_ptr<pq_dal::ConnectionsPool> PQConnectionPoolPtr;
}  // space


#endif
