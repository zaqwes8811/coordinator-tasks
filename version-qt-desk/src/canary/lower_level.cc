#include "top/config.h"

#include "canary/lower_level.h"

namespace lower_level {
PQConnectionPool::PQConnectionPool(const std::string& conn_info)
  : conn_(new pqxx::connection(conn_info)) {
  assert(conn_->is_open());
}

PQConnectionPool::~PQConnectionPool() {
  try {
    conn_->disconnect();
  } catch (...) {}
}

namespace psql_space {
using std::string;
using pqxx::connection;
using pqxx::work;

// можно было использовать ссылку и ByRef()
void rm_table(connection& conn, const string& table_name)
{
  // Если таблицы нет, то просто ничего не происходит.
  string sql("drop table " + table_name + ";");

  // создаем транзакционный объект
  work w(conn);

  // Exec
  w.exec(sql);
  w.commit();
}

void run_transaction(const string& sql, /*const*/ connection& C)
{
  work W(C);
  W.exec(sql);
  W.commit();
}
}  // ns
}
