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

ChainFilters::ChainFilters() {
  //l_.push_back(make_shared<EmptyFilter>(EmptyFilter()));  // все же не нужно
}

void ChainFilters::add(FilterPtr e)
{ l_.push_back(e); }

// FIXME: как удалить то без RTTI? Список то полиморфный
void ChainFilters::remove(FilterPtr e)
{ l_.remove(e); }

entities::Tasks ChainFilters::operator()(entities::Tasks e) const {
  entities::Tasks r = e;  // impl. empty filter

  // фильтруем
  for (std::list<FilterPtr>::const_iterator it = l_.begin(); it != l_.end(); ++it) {
    FilterPtr action = *it;
    r = (*action)(r);
  }

  return r;
}

}  // namespace
