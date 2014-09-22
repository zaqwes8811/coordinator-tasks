#include "top/config.h"

#include "canary/storage_access.h"
#include "canary/entities.h"

// C++
#include <iostream>

// C
#include <cassert>

namespace dal {
using std::string;
using std::cout;
using std::endl;

using pqxx::connection;
using pqxx::work;
using pqxx::result;
using pqxx::nontransaction;

using domain::Task;

void TaskTableQueries::printTable(connection& C) const {
  /* Create a non-transactional object. */
  nontransaction N(C);

  /* Execute SQL query */
  string sql = "SELECT * from " + table_name_ + ";";
  result R( N.exec( sql ));

  /* List down all the records */
  for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
    cout << "ID = " << c[0].as<int>() << " Name = " << c[1].as<string>() << " Priority = " << c[2].as<int>() << endl;
  }
}

void TaskTableQueries::createTable(connection& C) {
  string sql = string("CREATE TABLE ") +
    "IF NOT EXISTS "+ // v9.1 >=
    table_name_ +
    "(" \
    // сделать чтобы было >0
    "id         SERIAL PRIMARY KEY NOT NULL," \
    "task_name  TEXT               NOT NULL, " \
    "priority   INT                NOT NULL);";

  psql_space::run_transaction(sql, C);
}

void TaskTableQueries::dropTable(connection& C) {
  psql_space::rm_table(C, table_name_);
}

Task& TaskLifetimeQueries::store(Task& task, connection& C) const {
  // нужно получить id
  // http://stackoverflow.com/questions/2944297/postgresql-function-for-last-inserted-id
  {
    // Insert
    string sql =
      "INSERT INTO " + table_name_ + " (task_name, priority) " \
        "VALUES ('"+task.get_task_name()+"', 32) RETURNING id; ";

    work W(C);
    result R( W.exec( sql ));
    W.commit();

    // Узнаем что за ключ получили
    int current_id = domain::Task::kInActiveKey;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
      current_id = c[0].as<int>();
      break;
    }

    assert(current_id != domain::Task::kInActiveKey);

    task.set_primary_key_(current_id);
  }
  return task;
}
}

namespace psql_space {
using std::string;
using pqxx::connection;
using pqxx::work;

// можно было использовать ссылку и ByRef()
void rm_table(connection& C, const string& table_name)
{
  // Если таблицы нет, то просто ничего не происходит.
  string sql = string("drop table " + table_name + ";");

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

