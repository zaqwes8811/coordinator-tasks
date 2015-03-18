// Хрень с пространствами имен. 
// http://rsdn.ru/forum/cpp/2141920.all
// Проблема в циклической зависимости
#include <typeinfo>

#include "heart/config.h"

#include "data_access_layer/postgresql_queries.h"
#include "model_layer/entities.h"

#include <loki/ScopeGuard.h>
#include <pqxx/pqxx> 
#include <gtest/gtest.h>

#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>

#include <cassert>

namespace {
using namespace pq_dal;
using namespace entities;
using namespace Loki;
using namespace pqxx;
using namespace std;

void do_something(gc::WeakPtr<pqxx::connection> C)
{
  using models::kTaskTableNameRef;
  
  // Tasks
  TaskTableQueries q(kTaskTableNameRef, C);
  q.registerBeanClass();  // clang segfault

  cout << "create table\n";

  // Если не создано, то нет смысла
  // а если не создасться? Тут похоже все равно.
  auto table_guard = MakeObjGuard(q, &TaskTableQueries::drop);

  // Create records
  TaskLifetimeQueries q_insert(kTaskTableNameRef, C);
  auto t = Task();
 // q_insert.create(t, C);
 // assert(t.get_primary_key() != EntitiesStates::kInActiveKey);
 // q_insert.create(t, C);

  // Tags

  // View
  //q.draw(cout);
}

TEST(postgres, all) {
  auto C = std::make_shared<connection>(models::kConnection);
  cout << "conn" << endl;
  {
    if (!C->is_open()) {
      throw runtime_error("Can't open database");
    }
    
    auto guard = MakeObjGuard(*C, &connection::disconnect);
    EXPECT_NO_THROW(do_something(C));
  }
  assert(!C->is_open());
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
