#include "heart/config.h"

#include "view/renders.h"

namespace renders {
std::ostream& operator<<(std::ostream& o, entities::TaskEntity& a) {
  o << "Id: " << a.get_primary_key()
    << " TaskName: " << a.get_task_name()
    << std::endl;
  return o;
}
}
