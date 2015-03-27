// сперва создаем в ОЗУ, только потом сохраняем
//
// TROUBLE: 
//   что-то не то при работе с shared_ptrs
//   http://forum.ixbt.com/topic.cgi?id=26:40107
// nested http://www.boost.org/doc/libs/1_56_0/libs/bind/bind.html#nested_binds
//
// без boost функциональное программировать для stl какое-то неполноценное
//
// Save partial predicatd... Done - boost::functionI
// Operation composition... Done - boost::bind, own make troubles if work with shared ptrs
// vector<weak_ptrs>... и с bind будут проблемы - нужно как-то распаковывать элементы
//
// Summary:
//   Without boost fp looks like shit!!
//
//#define BOOST_BIND_ENABLE_STDCALL
//#define BOOST_BIND_ENABLE_FASTCALL   // win only
//
// Details:
//
//boost::function1<bool, shared_ptr<TaskEntity> > 
//p = std::not1(boost::make_adaptable<bool, shared_ptr<TaskEntity> >(p));
//p = bind(not1<>(), _1);  // not это не то
//
// если сложные выборки, то возможно лучше обычный цикл - см. Мейсера

#include "heart/config.h"

#include "model_layer/entities.h"
#include "data_access_layer/postgresql_queries.h"
#include "view/renders.h"
#include "data_access_layer/fake_store.h"
#include "model_layer/filters.h"
#include "common/app_types.h"

#include <gtest/gtest.h>
#include <loki/ScopeGuard.h>
#include <pqxx/pqxx> 

#include <algorithm>
#include <cassert>
#include <functional>
#include <stdexcept>
#include <vector>
#include <memory>

namespace {
using namespace pq_dal;
using namespace entities;
using namespace Loki;
using namespace pqxx;
using namespace fake_store;
using entities::TaskEntities;

using std::vector;
using std::string;
using std::cout;
using std::equal_to;
using renders::operator <<;

TEST(ModelTest, BaseCase) {
  using namespace std::placeholders;
  typedef vector<gc::WeakPtr<Task> > ModelWeakSlice;

  // пока храним все в памяти - активные только
  // ссылки не должны утечь, но как удалять из хранилища?
  TaskEntities model;

  model.push_back(std::make_shared<Task>());

  // only tmp!!! maybe weak? - тогда копия не владеет, хотя и работать не очень удобно
  // weak_ptr - неожиданно влядеет
  ModelWeakSlice query(model.size());  // слабый похоже не сработает
  copy(model.begin(), model.end(), query.begin());  // работает со слабым

  for_each(model.begin(), model.end(), bind(&Task::id, _1));

  EXPECT_TRUE(0 == query.at(0).lock()->priority);
}

TEST(ModelTest, Create) {
  // load from store
  TaskEntities model(get_all());
  
  // view unsaved
  cout << model;  

  // save
  auto C = std::make_shared<connection>(models::kConnection);
  {
    using models::kTaskTableNameRef;

    EXPECT_TRUE(C->is_open());
    
    auto conn_guard = MakeObjGuard(*C, &connection::disconnect);

    // Tasks
    TaskTableQueries q(kTaskTableNameRef, C);
    q.registerBeanClass();
    // Если не создано, то нет смысла
    // а если не создасться? Тут похоже все равно.
    auto table_guard = MakeObjGuard(q, &TaskTableQueries::drop);
    
    {
      // Create records
      TaskLifetimeQueries q_insert(kTaskTableNameRef, C);
      //q_insert.create(model, C);

      //Tasks::iterator it = adobe::find_if(model, filters::get_check_non_saved());

      //EXPECT_EQ(it, model.end());  // все сохранили и исключение не выскочило

      // View
      //q.draw(cout);
    }
  }
  EXPECT_FALSE(C->is_open());

}

TEST(Values, Assign) {
  using namespace entities;
  Task v;// = Task::create();
  Task v1;// = Task::create("hello", 90);
  v = v1;
}

}
