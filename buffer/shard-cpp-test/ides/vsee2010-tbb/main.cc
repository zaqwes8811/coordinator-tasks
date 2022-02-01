// http://habrahabr.ru/post/155467/

// http://habrahabr.ru/post/118354/ - Рашсшинение студии но в 2010 что-то не работает
// http://stackoverflow.com/questions/2900818/how-to-organize-external-tools-in-visual-studio-2010
// Нужно переключиться на Expert settings

#include "app/config.h"

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

