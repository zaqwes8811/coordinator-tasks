//#include <typeinfo>

#include "top/config.h"

#include "canary/entities_and_values.h"
#include "canary/model.h"
#include "canary/filters.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace {
using boost::shared_ptr;
using boost::make_shared;
using filters::Filter;

class EmptyFilter : public Filter {
public:
  entities::Tasks operator()(entities::Tasks e) {
  }

  int get_type_id() const
  { return 1; }
};

class DoneFilter : public Filter {
public:
  entities::Tasks operator()(entities::Tasks e) {
  }
};

// сырые указатели лучше не передавать.
bool operator==(const Filter& lhs, const Filter& rhs) {
  // FIXME: It is very bad! dynamic cast don't work, no info
  return (typeid(lhs)) == (typeid(rhs));
}

bool operator==(Filter::FilterPtr lhs, Filter::FilterPtr rhs) {
  return typeid(*lhs) == typeid(*rhs);
}


// на входе весь кеш, на выходе результат собственно, может лучше через SQL?
// Но как легко комбинировать фильтры. Откат к sql может повлиять, а может и нет на архитектуры.
//
// зацепаемся за типы? может еще можно как-то?
class ChainFilters {
public:
  ChainFilters() {
    //l_.push_back(make_shared<EmptyFilter>(EmptyFilter()));  // все же не нужно
  }

  void add(Filter::FilterPtr e)
  { l_.push_back(e); }

  // FIXME: как удалить то без RTTI? Список то полиморфный
  void remove(Filter::FilterPtr e)
  { l_.remove(e); }

  entities::Tasks operator()(entities::Tasks e) const {
    entities::Tasks r = e;  // impl. empty filter

    return r;
  }

private:
  std::list<Filter::FilterPtr> l_;
};

TEST(ChainFilter, Base) {
  Filter* f = new EmptyFilter;
  DoneFilter* e = dynamic_cast<DoneFilter*>(f);
  EXPECT_FALSE(e);

  EmptyFilter* e_ = dynamic_cast<EmptyFilter*>(f);
  EXPECT_TRUE(e_);
  delete f;
}
}  // namesp..
