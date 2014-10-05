//#include <typeinfo>

#include "top/config.h"

#include "canary/entities_and_values.h"
#include "canary/model.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
using models::Filter;

class EmptyFilter : public Filter {
public:
  entities::Tasks operator()(entities::Tasks e) {
  }
};

class SortFilter : public Filter {
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
  // FIXME: It is very bad! dynamic cast don't work, no info
  return (typeid(*lhs)) == (typeid(*rhs));
}


// на входе весь кеш, на выходе результат собственно, может лучше через SQL?
// Но как легко комбинировать фильтры.
//
// зацепаемся за типы? может еще можно как-то?
class ChainFilters {
public:
  void add(models::Filter::FilterPtr e);

  // FIXME: как удалить то без RTTI? Список то полиморфный
  void remove(models::Filter::FilterPtr e);

  void operator()() const;

};

TEST(ChainFilter, Base) {
  Filter* f = new EmptyFilter;
  SortFilter* e = dynamic_cast<SortFilter*>(f);
  EXPECT_FALSE(e);

  EmptyFilter* e_ = dynamic_cast<EmptyFilter*>(f);
  EXPECT_TRUE(e_);
}
