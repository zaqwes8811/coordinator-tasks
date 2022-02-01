#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>

#if __cplusplus >= 201103L
#  include <thread>
#endif

// BUG: boost::thread + main thread races not detected
// BUG: but by boost::ref not detected

// not work
void f1(int& X) {
  X = 42;
}

void f2(int* X) {
  *X = 42;
}

void f3(boost::shared_ptr<int> X) {
  *X = 42;
}

TEST(TSanBoostRefTest, NonDetectRaceOnPutRef) {
  int X;
  // not working
  boost::thread t(boost::bind(&::f1, boost::ref(X)));

  // Main thread work
  X = 43;  // not see

  t.join();
}

// If with filter not failed
// single test not work NonDetectRaceWithMainThread
TEST(TSanBoostRefTest, NonDetectRaceWithMainThread) {
  int X;
  boost::thread t(boost::bind(&::f2, &X));

  // Main thread work
  X = 43;  // not see
  f2(&X);  // not see
  f1(X);  // not see

  t.join();
}

#if __cplusplus >= 201103L
TEST(TSanBoostRefTest, DetectWithRefUseLambda) {
  int X;

  boost::thread_group g; // if threads in group work
  g.create_thread([&] { X = 89; });
  // Main thread work
  X = 43;

  g.join_all();
}
#endif

TEST(TSanBoostRefTest, DetectWorking) {
  int X;

  boost::thread t(boost::bind(&::f1, boost::ref(X)));
  boost::thread tt(boost::bind(&::f1, boost::ref(X)));

  t.join();
  tt.join();
}

