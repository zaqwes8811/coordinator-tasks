#ifndef DAL_H
#define DAL_H

#include "canary/entities_and_values.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pqxx/pqxx>

#include <string>
#include <vector>
#include <cassert>

namespace pq_lower_level {
void rm_table(pqxx::connection& C, const std::string& table_name);
void run_transaction(const std::string& sql, /*const*/ pqxx::connection& C);
}

namespace pq_dal {
class PQConnectionPool : public boost::noncopyable {
public:
  explicit PQConnectionPool(const std::string& conn_info);
  ~PQConnectionPool();

  // non-const
  boost::shared_ptr<pqxx::connection> get() {
      return m_conn_ptr;
  }
private:
  boost::shared_ptr<pqxx::connection> m_conn_ptr;
};

typedef boost::shared_ptr<pq_dal::PQConnectionPool> PQConnectionPoolPtr;


class TaskTableQueries : public boost::noncopyable
{
public:
  TaskTableQueries(const std::string& name, pqxx::connection* p)
    : m_table_name(name)
    , m_conn_ptr(p) { }

  void createIfNotExist();
  void drop();

  void draw(std::ostream& o) const;

private:
  const std::string m_table_name;
  pqxx::connection* const m_conn_ptr;
};

// Делать один репозиторий не советуют
// TODO: Может DI какой сделать, или все равно?
// FIXME: как то объединить create, update, etc. в persist
//
// Назначет id!! очень важно! объекты уникальные
// Создает, если еще не был создан, либо обновляет всю запись
// by value
// На групповую вставку могут быть ограничения, но в данной задаче
//   пока не нужно, если не нужно будет что-то куда-то автоматически переливать.
//
// FIXME: с умными указателями возникают проблемы с константростью!
class TaskLifetimeQueries
    : public boost::noncopyable
{
public:
   TaskLifetimeQueries(const std::string& table_name
                               , pqxx::connection* p);

  // values op.
  values::ImmutableTask create(const values::ImmutableTask& v);

  void update(const values::ImmutableTask& v);

  // entities op.
  entities::Tasks get_all() const;

private:
  const std::string m_table_name;
  pqxx::connection* const m_conn_ptr;
};
}  // ns


#endif // DAL_H
