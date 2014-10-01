#include "top/config.h"

#include "canary/filters.h"

#include <boost/bind.hpp>

//#include <

namespace filters {
// add check non saved
using boost::bind;
using std::equal_to;
using entities::EntitiesStates;
using entities::TaskEntity;
//using boost::bind::_1;

boost::function1<bool, entities::Tasks::value_type> get_check_non_saved() {
  return bind(
      bind(equal_to<int>(), _1, EntitiesStates::kInActiveKey),
      bind(&TaskEntity::get_primary_key, _1)) ;
}

boost::function1<bool, entities::Tasks::value_type> get_check_contained(const int id) {
  return bind(
      bind(equal_to<int>(), _1, id),
      bind(&TaskEntity::get_primary_key, _1)) ;
}

}  // namespace
