#include "top/config.h"

#include "canary/entities.h"

#include <gtest/gtest.h>
#include <boost/shared_ptr.hpp>

#include <vector>

namespace {
using namespace domain;
using namespace std;
using namespace boost;

TEST(Model, BaseCase) {
  vector<shared_ptr<Task> > model;
}
}