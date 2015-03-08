// Хрень с пространствами имен. 
// http://rsdn.ru/forum/cpp/2141920.all
// Проблема в циклической зависимости
#include <typeinfo>

#include "top/config.h"

#include "dal/pq_queries.h"
#include "canary/entities_and_values.h"

#include <loki/ScopeGuard.h>
#include <pqxx/pqxx> 
#include <gtest/gtest.h>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

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
//using namespace pq_lower_level;
using namespace std;

void do_something(boost::weak_ptr<pqxx::connection> C)
{
  using models::kTaskTableNameRef;
  
  // Tasks
  TaskTableQueries q(kTaskTableNameRef, C);
  q.createIfNotExist();  // clang segfault

  cout << "create table\n";

  // Если не создано, то нет смысла
  // а если не создасться? Тут похоже все равно.
  ScopeGuard table_guard = MakeObjGuard(q, &TaskTableQueries::drop);

  // Create records
  TaskLifetimeQueries q_insert(kTaskTableNameRef, C);
  TaskEntity t;
 // q_insert.create(t, C);
 // assert(t.get_primary_key() != EntitiesStates::kInActiveKey);
 // q_insert.create(t, C);

  // Tags

  // View
  q.draw(cout);
}

TEST(postgres, all) {
  auto C = boost::make_shared<connection>(models::kConnection);
  cout << "conn" << endl;
  {
    if (!C->is_open()) {
      throw runtime_error("Can't open database");
    }
    
    ScopeGuard guard = MakeObjGuard(*C, &connection::disconnect);
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
