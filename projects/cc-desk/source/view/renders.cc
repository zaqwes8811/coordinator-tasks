#include "heart/config.h"

#include "view/renders.h"

namespace renders {
std::ostream& operator<<(std::ostream& o, entities::TaskEntity& a) {
  o << "Id: " << a.m_primaryKey
    << " TaskName: " << a.m_name
    << std::endl;
  return o;
}
}
