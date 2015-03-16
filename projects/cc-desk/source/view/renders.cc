#include "heart/config.h"

#include "view/renders.h"

namespace renders {
std::ostream& operator<<(std::ostream& o, entities::Task& a) {
  o << "Id: " << a.id
    << " TaskName: " << a.name
    << std::endl;
  return o;
}
}
