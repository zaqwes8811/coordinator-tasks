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
#include "canary/pq_dal.h"
#include "visuality/view.h"

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
using namespace psql_space;
using namespace std;
using namespace view;

struct Ref {

};

ostream& operator<<(ostream& o, TaskEntity& a) {
  o << "Id: " << a.get_primary_key() << " TaskName: " << a.get_task_name() << endl;
  return o;
}

template <class T>
class _ActionSmart {
public:
  explicit _ActionSmart(std::ostream* o_) : o(o_) { }
  void operator()(const boost::shared_ptr<T>& elem) const {
    *o << *elem;// << ", ";
  }
  std::ostream* const o;
};

template <class T>
ostream& operator<<(ostream& o, const vector<shared_ptr<T> >& a) 
{
  std::for_each(a.begin(), a.end(), 
      //view::ActionSmart<T>(&o));  // FIXME: не видит. 
      _ActionSmart<T>(&o));
  o << std::endl;
  return o;
}

const char* events[] = {
  "A weak_ptr can only be created from a shared_ptr,",
  "and at object construction time no shared_ptr to",
  "Even if you could create a temporary shared_ptr to this, ",
  "it would go out of scope at the end of the constructor, ",
  "and all weak_ptr instances would instantly expire."};

//const char* labels[] = {"v8", "fake"};

vector<shared_ptr<TaskEntity> > build_fake_model() {
  vector<shared_ptr<TaskEntity> > model;  

  for (int i = 0; i < 5; ++i) {
    string message(events[i]);
    shared_ptr<TaskEntity> tmp = TaskEntity::create(message);
    model.push_back(tmp);
  }

  return model;
}

TEST(Model, BaseCase) {
  typedef vector<shared_ptr<TaskEntity> > Model;
  typedef vector<weak_ptr<TaskEntity> > ModelWeakSlice;

  // пока храним все в памяти - активные только
  // ссылки не должны утечь, но как удалять из хранилища?
  Model model;  

  model.push_back(
    make_shared<Model::value_type::element_type>(
      Model::value_type::element_type()));

  // only tmp!!! maybe weak? - тогда копия не владеет, хотя и работать не очень удобно
  // weak_ptr - неожиданно влядеет
  ModelWeakSlice query(model.size());  // слабый похоже не сработает
  copy(model.begin(), model.end(), query.begin());  // работает со слабым

  for_each(model.begin(), model.end(), bind(&TaskEntity::get_primary_key, _1));

  assert(0 == query.at(0).lock()->get_priority());
}

TEST(Model, Create) {
  typedef vector<shared_ptr<TaskEntity> > Model;
  // load from store
  Model model(build_fake_model());
  
  // view unsaved
  cout << model;  

  // save
  connection C(app::kConnection);
  {
    using app::kTableName;

    EXPECT_TRUE(C.is_open());
    
    ScopeGuard conn_guard = MakeObjGuard(C, &connection::disconnect);

    // Tasks
    TaskTableQueries q(kTableName);
    q.createTable(C);
    // Если не создано, то нет смысла
    // а если не создасться? Тут похоже все равно.
    ScopeGuard table_guard = MakeObjGuard(q, &TaskTableQueries::dropTable, ByRef(C));
    
    {
      // Create records
      TaskLifetimeQueries q_insert(kTableName);

      q_insert.persist(model, C);

      // не должно быть пустышек
      boost::function1<bool, Model::value_type> p =  
        bind(
          bind(equal_to<int>(), _1, EntitiesStates::kInActiveKey), 
          bind(&TaskEntity::get_primary_key, _1)) ;

      Model::iterator it = adobe::find_if(model, p);

      EXPECT_EQ(it, model.end());  // все сохранили и исключение не выскочило

      // View
      q.printTable(C);
    }
  }
  EXPECT_FALSE(C.is_open());

}

}
