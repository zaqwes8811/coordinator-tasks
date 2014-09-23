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
//#define BOOST_BIND_ENABLE_STDCALL
//#define BOOST_BIND_ENABLE_FASTCALL   // win only

#include "top/config.h"

#include "canary/entities.h"
#include "canary/storage_access.h"
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
//#include <boost/tr1/functional.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <stdexcept>

namespace dirty {
using namespace domain;

// TROUBLES: при работе с shared_ptrs
template <class OP1, class OP2
  //, typename R
>
class compose_f_gx_t
  //: std::unary_function<typename R /*OP2/*::argument_type*/, typename OP1::result_type> 
  {
private:
  OP1 op1;
  OP2 op2;

public:
  compose_f_gx_t(const OP1& o1, const OP2& o2) : op1(o1), op2(o2) { }

  template <typename R>
  typename OP1::result_type
  operator()(
    //const typename OP2::argument_type
    R& x) const {  //
    typename OP1::result_type tmp = op1(op2(x));
    return tmp;
  }
};

template <class OP1, class OP2> 
inline compose_f_gx_t<OP1, OP2>
compose_f_gx(const OP1& o1, const OP2& o2) {
  return compose_f_gx_t<OP1, OP2>(o1, o2);
}

template <class OP1>
class store_t
  : std::unary_function<typename OP1::argument_type, typename OP1::result_type> 
  {
private:
  OP1 op1;

public:
  explicit store_t(const OP1& o1) : op1(o1) { }

  typename OP1::result_type operator()(const typename OP1::argument_type& x) const {  //
    typename OP1::result_type tmp = op1(x);
    return tmp;
  }
};

template <class OP1> 
inline store_t<OP1>
store(const OP1& o1) {
  return store_t<OP1>(o1);
}

// создается лишний класс
struct is_saved {
  bool operator()(boost::shared_ptr<TaskEntity> e) {
    return true;
  }
};
}

namespace stolen {
// http://rsdn.ru/forum/cpp/342382.all
template<typename T>
struct smart2ptr_t
{
    //typedef X argument_type;  // FIXME: как сделать то?

    T f;

    smart2ptr_t(T t) : f(t) {}

    template<typename R>
    void operator()(R &p)
    {
        f(p.get());
    }
};

template<typename T>
smart2ptr_t<T> smart2ptr(T op)
{
    return smart2ptr<T>(op);
}
}

namespace {
using namespace boost;
using namespace dal;
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
  "and at object construction time no shared_ptr to the object exists yet. ",
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
  // пока храним все в памяти - активные только
  // ссылки не должны утечь, но как удалять из хранилища?
  vector<shared_ptr<TaskEntity> > model;  

  model.push_back(make_shared<TaskEntity>(TaskEntity()));

  // only tmp!!! maybe weak? - тогда копия не владеет, хотя и работать не очень удобно
  vector<weak_ptr<TaskEntity> > query(model.size());  // слабый похоже не сработает
  copy(model.begin(), model.end(), query.begin());  // работает со слабым

  assert(0 == query.at(0).lock()->get_priority());
}

template <typename Auto>
struct auto_cont {
  Auto a;
  explicit auto_cont(Auto& _a) : a(_a) {}
  Auto get() const { return a; }
};

template <typename A> 
auto_cont<A> inline get_a(A& a) {
  return auto_cont<A>(a);
}

TEST(Model, Create) {
  typedef vector<shared_ptr<TaskEntity> > Model;
  // load from store
  vector<shared_ptr<TaskEntity> > model(build_fake_model());
  
  // view unsaved
  cout << model;  

  // save
  connection C(app::kConnection);
  {
    using app::kTableName;

    if (!C.is_open()) {
      throw runtime_error("Can't open database");
    }
    
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
      boost::function1<bool, shared_ptr<TaskEntity> > p =  
        bind(
          bind(equal_to<int>(), _1, EntitiesStates::kInActiveKey), 
          bind(&TaskEntity::get_primary_key, _1)) ;

      //boost::function1<bool, shared_ptr<TaskEntity> > 
      p = std::not1(boost::make_adaptable<bool, shared_ptr<TaskEntity> >(p));

      Model::iterator it = adobe::find_if(model, p);

      assert(it == model.end());  // все сохранили и исключение не выскочило

      // View
      q.printTable(C);
    }
  }
  assert(!C.is_open());

}

}