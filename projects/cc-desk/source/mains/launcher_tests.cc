#include "heart/config.h"

#include "core/scopes.h"

#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
  scopes::AppScope _;

  testing::InitGoogleTest(&argc, argv);
  testing::GTEST_FLAG(print_time) = true;
  return RUN_ALL_TESTS();
}
