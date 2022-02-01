// http://habrahabr.ru/post/155467/
#include <iostream>
#include <string>

#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
 // argv[1] = "--gtest_filter=InMemoryStorage.*"; argc = 2;
 
  // Run
  testing::InitGoogleTest(&argc, argv);
  testing::GTEST_FLAG(print_time) = true;
  RUN_ALL_TESTS();
  return 0;
}

