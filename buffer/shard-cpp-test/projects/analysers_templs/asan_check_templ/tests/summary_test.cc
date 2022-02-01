
#include <gtest/gtest.h>

struct s {
  int f(int argc) {
    int* array = new int[100];
    delete [] array;
    return array[argc];  // +100 not working
  }
};

static int ret;
TEST(ASanTest, HeapAfterFree) {
  ret = s().f(1);
}

TEST(ASanTest, Stack) {
  int stack_array[100] = {-1};
  //return stack_array[argc + 100];
  ret = stack_array[1 + 100];  // +100 not working
}

// ASAN_OPTIONS=detect_stack_use_after_return=1
// http://www.chromium.org/developers/testing/leaksanitizer
static int * g;

void LeakLocal() {
	int local;
	g = &local;
}

TEST(ASanTest, StackUseAfter) {
  LeakLocal();
  ret = *g;
}

// Need options
// ASAN_OPTIONS="detect_leaks=1 symbolize=1" out/Release/base_unittests
// FIXME: must work
//   https://code.google.com/p/address-sanitizer/wiki/LeakSanitizer

namespace {
static int *g = new int;

TEST(ASanTest, Leak) {
  g = 0;
  //int *g = new int;
}
}

TEST(ASanTest, HeapOvf) {
  int* array = new int[100];
  int res = array[1 + 100];
  delete [] array;
  ret = res;//stack_array[argc + 100];  // +100 not working
}

namespace {
int global_array[100] = {-1};
int ret = 0;
TEST(ASanTest, Glob2) {
  //return global_array[argc + 100];
  ret = global_array[1 + 5 + 100];  // +100 not working
}
}




