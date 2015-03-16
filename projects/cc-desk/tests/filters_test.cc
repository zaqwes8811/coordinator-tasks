#include "heart/config.h"

#include "model_layer/entities.h"
#include "model_layer/model.h"
#include "model_layer/filters.h"

#include <gtest/gtest.h>

namespace {
using filters::Filter;
using filters::FilterPtr;
using entities::Task;
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
