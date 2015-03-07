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
using filters::DoneFilter;
using filters::SortByPriorityFilter;

TEST(ChainFilter, Base) {
  Filter* f = new DoneFilter;
  SortByPriorityFilter* e = dynamic_cast<SortByPriorityFilter*>(f);
  EXPECT_FALSE(e);

  DoneFilter* e_ = dynamic_cast<DoneFilter*>(f);
  EXPECT_TRUE(e_);
  delete f;
}
}  // namesp..
