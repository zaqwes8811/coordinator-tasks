#ifndef CANARY_APP_TYPES_H
#define CANARY_APP_TYPES_H

#include <memory>

namespace gc {

// http://en.cppreference.com/w/cpp/language/type_alias
template<typename T> using SharedPtr =
  //boost
  std
  ::shared_ptr<T>;
template<typename T> using WeakPtr =
  std
  //boost
  ::weak_ptr<T>;

// Trouble with xxx::make_shared<T>
//   http://stackoverflow.com/questions/19572512/c11-template-function-aliases-vs-wrappers

}

#endif // CANARY_APP_TYPES_H
