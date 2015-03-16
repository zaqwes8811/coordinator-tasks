#include "heart/config.h"

#include "model_layer/filters.h"

#include <string>
#include <algorithm>

namespace filters {
using namespace std::placeholders;
using std::bind;
using std::equal_to;
using entities::EntityStates;
using entities::TaskEntity;

using std::string;

std::function<bool(entities::Tasks::value_type)> is_non_saved() {
  return bind(
      bind(equal_to<size_t>(), _1, EntityStates::kInactiveKey),
      bind(&TaskEntity::getId, _1)) ;
}

std::function<bool(entities::TaskEntityPtr)> is_contained(const int id) {
  return bind(
      bind(equal_to<size_t>(), _1, id),
      bind(&TaskEntity::getId, _1)) ;
}

std::function<bool(entities::Tasks::value_type)> get_is_non_done() {
  return bind(
      bind(equal_to<size_t>(), _1, entities::EntityStates::kNonDone),
      bind(&TaskEntity::getIsDone, _1)) ;
}

ChainFilters::ChainFilters() { }

void ChainFilters::add(FilterPtr e)
{ m_s.insert(e); }

// FIXME: как удалить то без RTTI? Список то полиморфный
void ChainFilters::remove(FilterPtr e)
{ m_s.erase(e); }

entities::Tasks ChainFilters::operator()(entities::Tasks e) const {
  auto r = e;
  for (auto& action: m_s)  r = (*action)(r);
  return r;
}

entities::Tasks DoneFilter::operator()(entities::Tasks e)
{
  auto it = std::stable_partition(e.begin(), e.end(), get_is_non_done());
  return entities::Tasks(e.begin(), it);
}

int DoneFilter::get_type_code() const
{ return 1; }


entities::Tasks SortByPriorityFilter::operator()(entities::Tasks e) {
  std::stable_sort(e.begin(), e.end(),
      bind(std::greater<int>(),
           bind(&TaskEntity::get_priority, _1),
           bind(&TaskEntity::get_priority, _2)));
  return e;
}

int SortByPriorityFilter::get_type_code() const
{ return 2; }

// сырые указатели лучше не передавать.
// FIXME: It is very bad! dynamic cast don't work, no info
//return (typeid(lhs)) == (typeid(rhs));
bool operator==(const Filter& lhs, const Filter& rhs) {
  return lhs.get_type_code() == rhs.get_type_code();
}

//return typeid(*lhs) == typeid(*rhs);  // no way
bool operator==(FilterPtr lhs, FilterPtr rhs) {
  return lhs->get_type_code() == rhs->get_type_code();
}

std::size_t hash_value(FilterPtr b)
{
    std::hash<int> hasher;
    return hasher(b->get_type_code());
}

entities::Tasks SortByTaskName::operator()(entities::Tasks e) {
  std::stable_sort(e.begin(), e.end(),
      bind(std::greater<string>(),
           bind(&TaskEntity::get_task_name, _1),
           bind(&TaskEntity::get_task_name, _2)));
  return e;
}

int SortByTaskName::get_type_code() const {
  return 3;
}


}  // namespace
