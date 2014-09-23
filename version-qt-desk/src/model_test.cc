#include "top/config.h"

#include "canary/entities.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>
#include <algorithm>
#include <cassert>

namespace {
using namespace domain;
using namespace std;
using namespace boost;

const char* events[] = {
  "A weak_ptr can only be created from a shared_ptr,",
  "and at object construction time no shared_ptr to the object exists yet. ",
  "Even if you could create a temporary shared_ptr to this, ",
  "it would go out of scope at the end of the constructor, ",
  "and all weak_ptr instances would instantly expire."};

const char* labels[] = {"v8", "fake"};

vector<shared_ptr<Task> > build_fake_model() {
  vector<shared_ptr<Task> > model;  

  for (int i = 0; i < 5; ++i) {
    string message(events[i]);
    shared_ptr<Task> tmp = Task::create(message);
    model.push_back(tmp);
  }

  return model;
}

TEST(Model, BaseCase) {
  // пока храним все в памяти - активные только
  // ссылки не должны утечь, но как удалять из хранилища?
  vector<shared_ptr<Task> > model;  

  model.push_back(make_shared<Task>(Task()));

  // only tmp!!! maybe weak? - тогда копия не владеет, хотя и работать не очень удобно
  vector<weak_ptr<Task> > query(model.size());  // слабый похоже не сработает
  copy(model.begin(), model.end(), query.begin());  // работает со слабым

  assert(0 == query.at(0).lock()->get_priority());
}

TEST(Model, Create) {
  vector<shared_ptr<Task> > model(build_fake_model());

}

}