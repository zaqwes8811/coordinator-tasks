#ifndef DAL_H
#define DAL_H

#include "canary/app_types.h"
#include "canary/entities.h"

// 3rdparty
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <pqxx/pqxx>

// C++
#include <string>
#include <vector>

/*
namespace domain {
// http://msdn.microsoft.com/en-us/library/0e5kx78b.aspx
//class TaskEntity;  // Forward reference; no definition
}*/

namespace pq_dal {
class TaskTableQueries : public boost::noncopyable {
public:
  TaskTableQueries(const std::string& name) : table_name_(name) { }
  void createTable(pqxx::connection& C);
  void dropTable(pqxx::connection& C);

  // http://stackoverflow.com/questions/17064297/overloading-operator-multiple-parameters
  //ostream& operator <<

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
  void persist(
      domain::Model tasks,
      pqxx::connection& C);  // const;  // no logic const.

  // Назначет id!
  //domain::TaskEntity& 
  void store(domain::TaskEntity& task, pqxx::connection& C) /*const*/;  // logical non-const

  //void removeById(int id);  // по сути не нужно

private:
  //TaskLifetimeQueries(const TaskLifetimeQueries&);
  //TaskLifetimeQueries& operator=(const TaskLifetimeQueries&);

  const std::string table_name_;
};
}  // ns

namespace psql_space {
// можно было использовать ссылку и ByRef()
void rm_table(pqxx::connection& C, const std::string& table_name);
void run_transaction(const std::string& sql, /*const*/ pqxx::connection& C);
}

#endif // DAL_H
