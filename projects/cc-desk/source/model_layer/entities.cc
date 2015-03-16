#include "heart/config.h"

#include "model_layer/entities.h"
#include "data_access_layer/pq_queries.h"

#include <vector>

namespace entities {
using namespace pq_dal;

const size_t EntityStates::kInactiveKey = -1;
const int EntityStates::kDefaultPriority = 0;
const bool EntityStates::kNonDone = false;

app::SharedPtr<TaskEntity> TaskEntity::create(const std::string& task_name)
{
  auto tmp = std::make_shared<TaskEntity>(TaskEntity());
  tmp->name = task_name;
  return tmp;
}

app::SharedPtr<TaskEntity> TaskEntity::create(const entities::TaskValue& v)
{
  auto tmp = TaskEntity::create(v.name);
  tmp->id = (v.id);
  tmp->priority = (v.priority);
  tmp->isDone = (v.isDone);
  return tmp;
}

void TaskEntity::assign(const entities::TaskValue& v) {
  id = v.id;
  name = v.name;
  priority = v.priority;
  isDone = v.isDone;
}

TaskEntity::TaskEntity()
  : id(EntityStates::kInactiveKey)
  , priority(EntityStates::kDefaultPriority)
  , isDone(false) { }

entities::TaskValue TaskEntity::toValue() const {
  return entities::TaskValue::create(
        id,
        name,
        priority,
        isDone);
}

}  // namespace

namespace entities {

TaskValue::TaskValue(const TaskValue& v)
  : id(v.id), name(v.name)
  , priority(v.priority), isDone(v.isDone) { }


TaskValue& TaskValue::operator=(const TaskValue& v) {
  TaskValue tmp(v);
  //std::swap(*this, tmp);  // failed on shared ptr - segfault

  std::swap(tmp.id, id);
  name = tmp.name;  // swap doesn't work, work now
  std::swap(tmp.priority, priority);
  std::swap(tmp.isDone, isDone);

  return *this;
}

TaskValue::TaskValue(const size_t _id, const std::string& d, const int p, const bool _d)
  : id(_id)
  , name(d)
  , priority(p)
  , isDone(_d)
{ }

TaskValue TaskValue::create(const std::string& d, const int p) {
  return TaskValue(entities::EntityStates::kInactiveKey, d, p, false);
}

TaskValue TaskValue::create(const size_t id, const std::string& d, const int p) {
  return TaskValue(id, d, p, false);
}

TaskValue TaskValue::create() {
  auto p = entities::EntityStates::kDefaultPriority;
  return TaskValue(entities::EntityStates::kInactiveKey, std::string(), p, false);
}

TaskValue TaskValue::create(
        const size_t id,
        const std::string& d,
        const int p,
        const bool _done) { return TaskValue(id, d, p, _done); }
}


