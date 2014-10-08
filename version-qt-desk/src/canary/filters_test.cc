//#include <typeinfo>

#include "top/config.h"

#include "canary/entities_and_values.h"
#include "canary/model.h"
#include "canary/filters.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <adobe/algorithm/sort.hpp>

namespace {
using boost::shared_ptr;
using boost::make_shared;
using boost::bind;
using filters::Filter;
using filters::FilterPtr;
using entities::TaskEntity;

class EmptyFilter : public Filter {
public:
  entities::Tasks operator()(entities::Tasks e)
  { return e; }

  int get_type_id() const
  { return 1; }
};

class SortByPriorityFilter : public Filter {
public:
  entities::Tasks operator()(entities::Tasks e) {
    adobe::stable_sort(e,
        bind(std::greater<int>(),
             bind(&TaskEntity::get_priority, _1),
             bind(&TaskEntity::get_priority, _2)));
    return e;
  }

  int get_type_id() const
  { return 2; }
};

/*
class DoneFilter : public Filter {
public:
  entities::Tasks operator()(entities::Tasks e) {
  }
};
*/

// сырые указатели лучше не передавать.
bool operator==(const Filter& lhs, const Filter& rhs) {
  // FIXME: It is very bad! dynamic cast don't work, no info
  //return (typeid(lhs)) == (typeid(rhs));
  return lhs.get_type_id() == rhs.get_type_id();
}

bool operator==(FilterPtr lhs, FilterPtr rhs) {
  //return typeid(*lhs) == typeid(*rhs);  // no way
  return lhs->get_type_id() == rhs->get_type_id();
}

TEST(ChainFilter, Base) {
  Filter* f = new EmptyFilter;
  SortByPriorityFilter* e = dynamic_cast<SortByPriorityFilter*>(f);
  EXPECT_FALSE(e);

  EmptyFilter* e_ = dynamic_cast<EmptyFilter*>(f);
  EXPECT_TRUE(e_);
  delete f;
}
}  // namesp..
