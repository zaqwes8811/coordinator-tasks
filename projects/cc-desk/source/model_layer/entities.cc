#include "heart/config.h"

#include "model_layer/entities.h"
#include "data_access_layer/pq_queries.h"

#include <vector>

namespace entities {
using namespace pq_dal;

const size_t EntityStates::kInactiveKey = -1;
const int EntityStates::kDefaultPriority = 0;
const bool EntityStates::kNonDone = false;

TaskEntity Task::create(const std::string& task_name)
{
  auto tmp = std::make_shared<Task>(Task());
  tmp->name = task_name;
  return tmp;
}

TaskEntity Task::create(const entities::Task& v)
{
  auto tmp = Task::create(v.name);
  tmp->id = (v.id);
  tmp->priority = (v.priority);
  tmp->isDone = (v.isDone);
  return tmp;
}

void Task::assign(const entities::Task& v) {
  id = v.id;
  name = v.name;
  priority = v.priority;
  isDone = v.isDone;
}

Task::Task()
  : id(EntityStates::kInactiveKey)
  , priority(EntityStates::kDefaultPriority)
  , isDone(false) { }

entities::Task Task::toValue() const {
  return entities::Task::create(
        id,
        name,
        priority,
        isDone);
}

}  // namespace

namespace entities {

Task::Task(const Task& v)
  : id(v.id), name(v.name)
  , priority(v.priority), isDone(v.isDone) { }


Task& Task::operator=(const Task& v) {
  Task tmp(v);
  //std::swap(*this, tmp);  // failed on shared ptr - segfault

  std::swap(tmp.id, id);
  name = tmp.name;  // swap doesn't work, work now
  std::swap(tmp.priority, priority);
  std::swap(tmp.isDone, isDone);

  return *this;
}

Task::Task(const size_t _id, const std::string& d, const int p, const bool _d)
  : id(_id)
  , name(d)
  , priority(p)
  , isDone(_d)
{ }

Task Task::create(const std::string& d, const int p) {
  return Task(entities::EntityStates::kInactiveKey, d, p, false);
}

Task Task::create(const size_t id, const std::string& d, const int p) {
  return Task(id, d, p, false);
}

Task Task::create() {
  auto p = entities::EntityStates::kDefaultPriority;
  return Task(entities::EntityStates::kInactiveKey, std::string(), p, false);
}

Task Task::create(
        const size_t id,
        const std::string& d,
        const int p,
        const bool _done) { return Task(id, d, p, _done); }
}


