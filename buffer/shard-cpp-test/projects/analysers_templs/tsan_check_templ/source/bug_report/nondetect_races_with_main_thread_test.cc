#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <gtest/gtest.h>

#if __cplusplus >= 201103L
#  include <thread>
#endif

// BUG: boost::thread + std::thread + main thread races not detected
// BUG: interference - depend on set cases

// FIXME: use boost::thread. Need rebuild boost and libstdc++?
// http://boost.2283326.n4.nabble.com/clang-3-6-thread-sanitizer-complains-on-shared-ptr-operations-td4671416.html
// Races not founded
//
// readelf -d a.out | grep NEEDED
// FIXME: may be wring linking?
//
// ldd - all right
/**
linux-vdso.so.1 =>  (0x00007fff9d0fe000)
    libboost_system.so.1.57.0 => /opt/big-3rdparty/boost_1_57_0/clang_libx64/lib/libboost_system.so.1.57.0 (0x00007f762d3a1000)
    libboost_thread.so.1.57.0 => /opt/big-3rdparty/boost_1_57_0/clang_libx64/lib/libboost_thread.so.1.57.0 (0x00007f762d174000)
    libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f762cf3f000)
    librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f762cd37000)
    libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f762cb33000)
    libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f762c82c000)
    libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f762c528000)
    libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f762c312000)
    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f762bf4b000)
    /lib64/ld-linux-x86-64.so.2 (0x00007f762ec9c000)
*/

// Contain boost ... () opertaor
// https://code.google.com/p/thread-sanitizer/issues/detail?id=55

// FIXME: old clang?
// FIXME: gcc?
// FIXME: can't detect

namespace {
void f1(int& X) {
  X = 42;
}
}


TEST(DetectTSanTest, BoostThreadInGrop) {
  int X;

  boost::thread_group g; // if threads in group work
  g.create_thread(boost::bind(&f1, boost::ref(X)));

  // Main thread work
  X = 43;

  g.join_all();
}

TEST(DetectTSanTest, TwoBoostThread) {
  int X;

  // failed but across this 2 threads
  boost::thread t(boost::bind(&::f1, boost::ref(X)));
  boost::thread tt(boost::bind(&::f1, boost::ref(X)));

  t.join();
  tt.join();
}

TEST(NonDetectTSanTest, RaceWithMainThread) {
  int X;
  boost::thread t(boost::bind(&::f1, boost::ref(X)));

  // Main thread work
  X = 43;  // not see
  f1(X);  // not see

  t.join();
}


#if __cplusplus >= 201103L
TEST(NonDetectTSanTest, StdThreadLambda) {
  int X;
  std::thread t([&] { X = 44; });

  // Main thread work
  X = 43;  // see

  t.join();
}

// single test non detect - in banch detect
TEST(NonDetectTSanTest, NonDetectBoostThreadLambda) {
  int X;
  boost::thread t([&] { X = 44; });

  // Main thread work
  X = 43;  // not see

  t.join();
}

// --gtest_filter=DetectTSanTest.*Boost*:
//   DetectTSanTest.*Std* - non detect if in banch with Boost.Thread
// --gtest_filter=DetectTSanTest.*Std* - detect
TEST(DetectTSanTest, StdThreadLambda) {
  int X;
  std::thread t([&] { X = 44; });

  // Main thread work
  X = 43;  // see

  t.join();
}

TEST(DetectTSanTest, StdThreadBoostBind) {
  int X;
  std::thread t(boost::bind(&f1, boost::ref(X)));

  // Main thread work
  X = 43;  // see

  t.join();
}
#endif
