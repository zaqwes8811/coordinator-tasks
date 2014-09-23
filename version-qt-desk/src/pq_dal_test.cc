// Хрень с пространствами имен. 
// http://rsdn.ru/forum/cpp/2141920.all
// Проблема в циклической зависимости
#include <typeinfo>

#include "top/config.h"

#include "canary/storage_access.h"
#include "canary/entities.h"

#include <loki/ScopeGuard.h>
#include <pqxx/pqxx> 
#include <gtest/gtest.h>

// C++
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>

// C
#include <cassert>

namespace {
using namespace dal;
using namespace domain;
using namespace Loki;
using namespace pqxx;
using namespace psql_space;
using namespace std;

void do_something(pqxx::connection& C)
{
  const string kTableName = "task_fake_entity";

  // Tasks
  TaskTableQueries q(kTableName);
  q.createTable(C);

  // Если не создано, то нет смысла
  // а если не создасться? Тут похоже все равно.
  ScopeGuard table_guard = MakeObjGuard(q, &TaskTableQueries::dropTable, ByRef(C));

  // Create records
  TaskLifetimeQueries q_insert(kTableName);
  TaskEntity t;
  q_insert.store(t, C);
  assert(t.get_primary_key() != EntitiesStates::kInActiveKey);
  q_insert.store(t, C);

  // Tags

  // View
  q.printTable(C);
}

TEST(postgres, all) {
  connection C("dbname=mydb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");
  {
    if (!C.is_open()) {
      throw runtime_error("Can't open database");
    }
    
    ScopeGuard guard = MakeObjGuard(C, &connection::disconnect);
    EXPECT_NO_THROW(do_something(C));
  }
  assert(!C.is_open());
}

TEST(postgres, error_codes) {
  try {

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
  }
}

}  // namespace
