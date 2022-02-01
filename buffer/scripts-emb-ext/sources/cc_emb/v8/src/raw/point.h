#ifndef POINT_H
#define POINT_H

namespace raw_objects {
// Accessing to dynamic vars.
// Point x y
class Point {
  public:
  Point() : x_(1), y_(2) { }
  Point(int x, int y) : x_(x), y_(y) { }
  int x_;
  int y_;
  //std::vector<int> rgb_;

  // cppclean failed here.
  //std::vector<vector<int>> rgb_add_;
};
}

#endif // POINT_H
