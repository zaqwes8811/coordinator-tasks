#ifndef RENDER_H
#define RENDER_H

#include "canary/entities.h"

#include <boost/shared_ptr.hpp>

#include <ostream>
#include <vector>
#include <algorithm>

namespace renders {
std::ostream& operator<<(std::ostream& o, domain::TaskEntity& a);

template <class T>
class _ActionSmart {
public:
  explicit _ActionSmart(std::ostream* o_) : o(o_) { }
  void operator()(const boost::shared_ptr<T>& elem) const {
    *o << *elem;// << ", ";
  }
  std::ostream* const o;
};

template <class T>
std::ostream& operator<<(std::ostream& o,
                         const std::vector<boost::shared_ptr<T> >& a)
{
  std::for_each(a.begin(), a.end(),
      //view::ActionSmart<T>(&o));  // FIXME: не видит.
      _ActionSmart<T>(&o));
  o << std::endl;
  return o;
}
}

#endif // RENDER_H
