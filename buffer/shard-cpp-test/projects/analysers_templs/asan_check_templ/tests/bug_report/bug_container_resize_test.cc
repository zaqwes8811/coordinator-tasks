#include <gtest/gtest.h>

#include <vector>

/**
Ubuntu clang version 3.4.2-3ubuntu2~xedgers (tags/RELEASE_34/dot2-final) (based on LLVM 3.4.2)
Target: x86_64-pc-linux-gnu
Thread model: posix
*/

namespace {
static int i;
TEST(ASanTest, Vec) {
  // http://stackoverflow.com/questions/19673311/stl-vector-resize-and-assign
  std::vector<int> V(8);
  V.resize(5);
  i = V.data()[6];
  //i = V[6];
}

TEST(ASanTest, VecStrange) {
  std::vector<int> V(8);
  V.resize(5);
  i = V.data()[7];
  i = V.at(6);//[6];
}
}

