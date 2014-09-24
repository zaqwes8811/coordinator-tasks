// Хрень с пространствами имен. 
// http://rsdn.ru/forum/cpp/2141920.all
// Проблема в циклической зависимости
#include <typeinfo>

#include "top/config.h"

#include "canary/pq_dal.h"
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
using namespace pq_dal;
using namespace domain;
using namespace Loki;
using namespace pqxx;
using namespace pq_lower_level;
using namespace std;

void do_something(pqxx::connection& C)
{
  using app::kTaskTableName;
  
  // Tasks
  TaskTableQueries q(kTaskTableName);
  q.createIfNotExist(C);

  // Если не создано, то нет смысла
  // а если не создасться? Тут похоже все равно.
  ScopeGuard table_guard = MakeObjGuard(q, &TaskTableQueries::drop, ByRef(C));

  // Create records
  TaskLifetimeQueries q_insert(kTaskTableName);
  TaskEntity t;
  q_insert.store(t, C);
  assert(t.get_primary_key() != EntitiesStates::kInActiveKey);
  q_insert.store(t, C);

  // Tags

  // View
  q.print(C);
}

TEST(postgres, all) {
  connection C(app::kConnection);
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
