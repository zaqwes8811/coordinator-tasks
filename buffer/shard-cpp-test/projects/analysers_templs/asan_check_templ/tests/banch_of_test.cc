#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

// FIXME: finded benchmark youtube video
// http://www.cookandcommit.eu/2014/09/tiny-c-benchmark-framework_29.html
// ...
//
// FIXME: my useful cases not working!!

//namespace {
static // index non static anyway
int global_array[100] = {-1};

int m(int argc) {
  return global_array[argc+100];
}

// not failed - looks code is optimized
TEST(Arg, A) {
  int i = 1;
  volatile int j = m(i);  // ignore
  //printf("%d", );  // ignore

  // ignore
  std::vector<int> v(8);
  int k = v[900];//.data()[900];
  ++k;
}

int val(int i) {
  //int j = 1;  // not working!!! why!?
  return m(i);
}

int get() {
  return 1;
}
//}

struct test {
  int j;

  test() : j(1) { }
};

// it's work - local var not
//static  // not work, only global
int j = 3;//argc;  // work but what the matter...

// FIXME: local and static can't be indexes?

TEST(ASanTest, Banch) {
  /// Case - local buffer
  // GotIt
  //char b[100];
  //return b[102];

  /// Case:
  // FIXME: if I can't use local vars then what?!
  //assert(argc == 1);
  //printf("%d", val(argc));
  //return val(argc);
  //static test t;  // with static work
  //int j = t.j;
  //int j = 1;  // don't catch if just digit
  //int j = get();  // don't work
  //int i = global_array[j+100];  // no protection after some shift +160 example
  //int i = val(j);  // no protection after some shift +160 example
  //return i;

  /// Case - heap buffer
  std::vector<int> v(8);
  int k = v[9];//.data()[900];  // if 900 - AddressSanitizer CHECK failed:
  printf("%d", k);  // no code line in GCC 4.8 and strange messages
  //return 0;//i;

  // FIXME: Run, but how
  //testing::InitGoogleTest(&argc, argv);
  //testing::GTEST_FLAG(print_time) = true;
  //return RUN_ALL_TESTS();
}


// FIXME: try check c-func call on configure posix sockets
