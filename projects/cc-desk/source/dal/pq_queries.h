#ifndef DAL_H
#define DAL_H

#include "canary/entities_and_values.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#ifdef G_USE_PSQL
#  include <pqxx/pqxx>
#endif

#include <string>
#include <vector>
#include <cassert>

namespace pq_lower_level {
// можно было использовать ссылку и ByRef()
void rm_table(pqxx::connection& C, const std::string& table_name);
void run_transaction(const std::string& sql, /*const*/ pqxx::connection& C);
}


namespace pq_dal {
class PQConnectionPool : public boost::noncopyable {
public:
  explicit PQConnectionPool(const std::string& conn_info);
  ~PQConnectionPool();

  // non-const
  boost::shared_ptr<pqxx::connection> get()
  {
      assert(conn_);
      return conn_;
  }
private:
  boost::shared_ptr<pqxx::connection> conn_;
};

typedef boost::shared_ptr<pq_dal::PQConnectionPool> PQConnectionPoolPtr;


class TaskTableQueries : public boost::noncopyable {
public:
  TaskTableQueries(const std::string& name) : table_name_(name) { }
  void createIfNotExist(pqxx::connection& C);
  void drop(pqxx::connection& C);
  void print(std::ostream& o, pqxx::connection& C) const;
private:
  const std::string table_name_;
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
  explicit TaskLifetimeQueries(const std::string& table_name);

  // values op.
  values::ImmutableTask create(const values::ImmutableTask& v, pqxx::connection& C);
  void update(const values::ImmutableTask& v, pqxx::connection& C);

  // entities op.
  entities::Tasks get_all(pqxx::connection& C) const;

private:
  const std::string _table_name;
};
}  // ns


#endif // DAL_H
