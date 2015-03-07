#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <sstream>

namespace common {
template <typename T>
std::string to_string(T const& value) {
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}
}


#endif // COMMON_H
