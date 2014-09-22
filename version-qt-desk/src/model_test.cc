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
}