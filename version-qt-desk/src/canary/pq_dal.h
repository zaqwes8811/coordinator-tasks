#ifndef DAL_H
#define DAL_H

#include "canary/entities.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pqxx/pqxx>

#include <string>
#include <vector>

namespace pq_dal {
class TaskTableQueries : public boost::noncopyable {
public:
  TaskTableQueries(const std::string& name) : table_name_(name) { }
  void createTable(pqxx::connection& C);
  void dropTable(pqxx::connection& C);
  void printTable(pqxx::connection& C) const;
private:
  const std::string table_name_;
};

// Делать один репозиторий не советуют
// TODO: Может DI какой сделать, или все равно?
class TaskLifetimeQueries : public boost::noncopyable {
public:
  explicit TaskLifetimeQueries(const std::string& table_name) : table_name_(table_name) {}

  // by value
  void persist(domain::Model tasks, pqxx::connection& C);

  // Назначет id!
  // logical non-const
  void store(domain::TaskEntity& task, pqxx::connection& C);
private:
  const std::string table_name_;
};
}  // ns

namespace psql_space {
// можно было использовать ссылку и ByRef()
void rm_table(pqxx::connection& C, const std::string& table_name);
void run_transaction(const std::string& sql, /*const*/ pqxx::connection& C);
}

#endif // DAL_H
