#include "heart/config.h"

#include "model_layer/filters.h"

#include <string>
#include <algorithm>

namespace filters {
using namespace std::placeholders;
using std::bind;
using std::equal_to;
using entities::EntityStates;
using entities::Task;

using std::string;

std::function<bool(entities::TaskEntity)> is_non_saved() {
  return bind(
      bind(equal_to<size_t>(), _1, EntityStates::kInactiveKey),
      bind(&Task::id, _1)) ;
}

std::function<bool(entities::TaskEntity)> is_contained(const size_t id) {
  return bind(bind(equal_to<size_t>(), _1, id),  bind(&Task::id, _1)) ;
}

static std::function<bool(entities::TaskEntity)> is_non_done() {
  return bind(
      bind(equal_to<size_t>(), _1, entities::EntityStates::kDefaultDone),
      bind(&Task::done, _1)) ;
}

ChainFilters::ChainFilters() { }

void ChainFilters::add(FilterPtr e)
{ m_s.insert(e); }

// FIXME: как удалить то без RTTI? Список то полиморфный
void ChainFilters::remove(FilterPtr e)
{ m_s.erase(e); }

entities::TaskEntities ChainFilters::operator()(entities::TaskEntities e) const {
  auto r = e;
  for (auto& action: m_s)  r = (*action)(r);
  return r;
}

entities::TaskEntities DoneFilter::operator()(entities::TaskEntities e)
{
  auto it = std::stable_partition(e.begin(), e.end(), is_non_done());
  return entities::TaskEntities(e.begin(), it);
}

int DoneFilter::typeCode() const
{ return 1; }


entities::TaskEntities SortByPriorityFilter::operator()(entities::TaskEntities e) {
  std::stable_sort(e.begin(), e.end(),
      bind(std::greater<int>(),
           bind(&Task::priority, _1),
           bind(&Task::priority, _2)));
  return e;
}

int SortByPriorityFilter::typeCode() const
{ return 2; }

// сырые указатели лучше не передавать.
// FIXME: It is very bad! dynamic cast don't work, no info
//return (typeid(lhs)) == (typeid(rhs));
bool operator==(const Filter& lhs, const Filter& rhs) {
  return lhs.typeCode() == rhs.typeCode();
}

//return typeid(*lhs) == typeid(*rhs);  // no way
bool operator==(FilterPtr lhs, FilterPtr rhs) {
  return lhs->typeCode() == rhs->typeCode();
}

std::size_t hash_value(FilterPtr b)
{
    std::hash<int> hasher;
    return hasher(b->typeCode());
}

entities::TaskEntities SortByTaskName::operator()(entities::TaskEntities e) {
  std::stable_sort(e.begin(), e.end(),
      bind(std::greater<string>(),
           bind(&Task::name, _1),
           bind(&Task::name, _2)));
  return e;
}

int SortByTaskName::typeCode() const {
  return 3;
}


}  // namespace
