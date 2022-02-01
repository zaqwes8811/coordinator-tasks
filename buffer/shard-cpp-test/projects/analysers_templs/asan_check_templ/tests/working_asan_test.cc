#include "heart/config.h"

#include <gtest/gtest.h>

// FIXME: not see it!!

int* g;

// ASAN_OPTIONS=detect_stack_use_after_return=1
namespace {
void LeakLocal() {
  int local;
  g = &local;
}

int* get() {
  int local;
  return &local;
}
}

int r() {
  int i = *get();
  LeakLocal();
  return *g;
}

namespace holder {
// else opt and not work - remove some code
static int fake_return_value = 0;
TEST(ASanTest, Mem)
{
  int *a = new int[100];
  delete[] a;

  int b = a[9];

  fake_return_value = b & 0x0f | b << 8;
}

int global_array[100] = {-1};

TEST(ASanTest, Global) {
  fake_return_value = global_array[1+100];
}

}
