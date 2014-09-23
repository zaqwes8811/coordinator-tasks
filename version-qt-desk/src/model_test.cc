// сперва создаем в ОЗУ, только потом сохраняем
//
//
#include "top/config.h"

#include "canary/entities.h"
#include "canary/storage_access.h"
#include "visuality/view.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <gtest/gtest.h>
#include <loki/ScopeGuard.h>
#include <pqxx/pqxx> 

#include <algorithm>
#include <cassert>
#include <vector>

namespace {
using namespace domain;
using namespace std;
using namespace boost;
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

TEST(Model, Create) {
  // load from store
  vector<shared_ptr<TaskEntity> > model(build_fake_model());
  
  // view unsaved
  cout << model;  

  // save
}

}