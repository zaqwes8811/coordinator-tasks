#ifndef RENDER_H
#define RENDER_H

#include "entities.h"

#include <ostream>
#include <vector>
#include <algorithm>

namespace renders {
std::ostream& operator<<(std::ostream& o, entities::Task& a);

template <class T>
class _ActionSmart {
public:
  explicit _ActionSmart(std::ostream* o_) : o(o_) { }
  void operator()(const gc::SharedPtr<T>& elem) const {
    *o << *elem;// << ", ";
  }
  std::ostream* const o;
};

template <class T>
std::ostream& operator<<(std::ostream& o,
                         const std::vector<gc::SharedPtr<T> >& a)
{
  std::for_each(a.begin(), a.end(),
      //view::ActionSmart<T>(&o));  // FIXME: не видит.
      _ActionSmart<T>(&o));
  o << std::endl;
  return o;
}

template <typename U>
void render_task_store(std::ostream& o, const U& a) {
  //a.draw_task_store(o);
}

}

#endif // RENDER_H
