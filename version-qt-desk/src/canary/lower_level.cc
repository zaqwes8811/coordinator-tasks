#include "top/config.h"

#include "canary/lower_level.h"

namespace lower_level { namespace psql_space {
using std::string;
using pqxx::connection;
using pqxx::work;

// можно было использовать ссылку и ByRef()
void rm_table(connection& C, const string& table_name)
{
  // Если таблицы нет, то просто ничего не происходит.
  string sql("drop table " + table_name + ";");

  // создаем транзакционный объект
  work W(C);

  // Exec
  W.exec(sql);
  W.commit();
}

void run_transaction(const string& sql, /*const*/ connection& C)
{
  work W(C);
  W.exec(sql);
  W.commit();
}
}  // ns
}
