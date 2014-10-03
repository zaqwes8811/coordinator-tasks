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

#include "top/config.h"

#include "canary/entities.h"
#include "canary/pq_queries.h"
#include "canary/renders.h"
#include "test_help_data.h"
#include "canary/filters.h"

#include <adobe/algorithm/find.hpp>  // удобно если работа с целым контейнером, иначе лучше std
#include <boost/bind.hpp>
#include <boost/bind/make_adaptable.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/mem_fn.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <gtest/gtest.h>
#include <loki/ScopeGuard.h>
#include <pqxx/pqxx> 
//#include <boost/tr1/functional.hpp>  // compilation problems

#include <algorithm>
#include <cassert>
#include <functional>
#include <stdexcept>
#include <vector>

namespace {
using namespace boost;
using namespace pq_dal;
using namespace domain;
using namespace Loki;
using namespace pqxx;
using namespace pq_lower_level;
using namespace test_help_data;
using domain::TasksMirror;

using std::vector;
using std::string;
using std::cout;
using std::equal_to;
using renders::operator <<;

TEST(ModelTest, BaseCase) {
  typedef vector<weak_ptr<TaskEntity> > ModelWeakSlice;

  // пока храним все в памяти - активные только
  // ссылки не должны утечь, но как удалять из хранилища?
  TasksMirror model;

  model.push_back(
    make_shared<TasksMirror::value_type::element_type>(
      TasksMirror::value_type::element_type()));

  // only tmp!!! maybe weak? - тогда копия не владеет, хотя и работать не очень удобно
  // weak_ptr - неожиданно влядеет
  ModelWeakSlice query(model.size());  // слабый похоже не сработает
  copy(model.begin(), model.end(), query.begin());  // работает со слабым

  for_each(model.begin(), model.end(), bind(&TaskEntity::get_primary_key, _1));

  assert(0 == query.at(0).lock()->get_priority());
}

TEST(ModelTest, Create) {
  // load from store
  TasksMirror model(build_fake_model());
  
  // view unsaved
  cout << model;  

  // save
  connection C(app::kConnection);
  {
    using app::kTaskTableNameRef;

    EXPECT_TRUE(C.is_open());
    
    ScopeGuard conn_guard = MakeObjGuard(C, &connection::disconnect);

    // Tasks
    TaskTableQueries q(kTaskTableNameRef);
    q.createIfNotExist(C);
    // Если не создано, то нет смысла
    // а если не создасться? Тут похоже все равно.
    ScopeGuard table_guard = MakeObjGuard(q, &TaskTableQueries::drop, ByRef(C));
    
    {
      // Create records
      TaskLifetimeQueries q_insert(kTaskTableNameRef);
      q_insert.create(model, C);

      TasksMirror::iterator it = adobe::find_if(model, filters::get_check_non_saved());

      //EXPECT_EQ(it, model.end());  // все сохранили и исключение не выскочило

      // View
      q.print(cout, C);
    }
  }
  EXPECT_FALSE(C.is_open());

}

}
