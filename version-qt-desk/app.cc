// Хрень с пространствами имен. 
// http://rsdn.ru/forum/cpp/2141920.all
// Проблема в циклической зависимости

#include <typeinfo>

// C
#include <cassert>

// C++
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>

// 3rdparty::Common
#include <loki/ScopeGuard.h>

// 3rdparty::Special
#include <pqxx/pqxx> 

// App
#include "version-qt-desk/storage_access.h"
#include "version-qt-desk/entities.h"

void do_something(pqxx::connection& C)
{
  using namespace std;

  using namespace Loki;
  using namespace dal;
  using namespace psql_space;
  using namespace domain;

  const string kTableName = "task_fake_entity";

  /// Tasks
  TaskTableQueries q(kTableName);
  q.createTable(C);

  // Если не создано, то нет смысла
  // а если не создасться? Тут похоже все равно.
  ScopeGuard table_guard = MakeObjGuard(q, &TaskTableQueries::dropTable, ByRef(C));

  // Create records
  TaskLifetimeQueries q_insert(kTableName);
  Task t;
  q_insert.store(t, C);
  assert(t.get_primary_key() != Task::kInActiveKey);
  q_insert.store(t, C);

  /// Tags

  /// View
  q.printTable(C);
}

int main() {
  using namespace std;
  using namespace pqxx;
  using namespace Loki;
  using namespace dal;
  using namespace psql_space;
  using namespace domain;

  try {
    connection C("dbname=mydb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");
    {
      if (!C.is_open()) {
	throw runtime_error("Can't open database");
      }
      
      ScopeGuard guard = MakeObjGuard(C, &connection::disconnect);
      do_something(C);
    }
    assert(!C.is_open());

  } catch (const pqxx::undefined_table& e) {
    // Нет таблицы
    cerr << typeid(e).name() << endl << e.what() << endl;
  } catch (const pqxx::unique_violation& e) {
    // Не уникальный ключ при вствке
    cerr << typeid(e).name() << endl << e.what() << endl;
  } catch (const pqxx::sql_error& e) {
    // Кажется ошибка в синтаксисе
    cerr << typeid(e).name() << endl << e.what() << endl;
  } catch (const std::exception& e) {
    cerr << "UNCATCHED" << endl;
    cerr << typeid(e).name() << endl;
    cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
