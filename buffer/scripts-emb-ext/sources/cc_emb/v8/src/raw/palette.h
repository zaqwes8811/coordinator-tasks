#ifndef PALETTE_H
#define PALETTE_H

// App
#include "raw/point.h"

namespace raw_objects {
class Palette {
 public:
  static const int MAX_SIZE = 32;
  Palette() {}
  int array_[MAX_SIZE];
  raw_objects::Point point_;
  raw_objects::Point point_array[MAX_SIZE];
};
}

#endif // PALETTE_H
