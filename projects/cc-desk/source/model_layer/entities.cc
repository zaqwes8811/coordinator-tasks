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
  tmp->m_name = task_name;
  return tmp;
}

app::SharedPtr<TaskEntity> TaskEntity::create(const entities::TaskValue& v)
{
  auto tmp = TaskEntity::create(v.m_name);
  tmp->id = (v.id);
  tmp->m_priority = (v.m_priority);
  tmp->m_isDone = (v.done);
  return tmp;
}

void TaskEntity::assign(const entities::TaskValue& v) {
  id = v.id;
  m_name = v.m_name;
  m_priority = v.m_priority;
  m_isDone = v.done;
}

TaskEntity::TaskEntity()
  : id(EntityStates::kInactiveKey)
  , m_priority(EntityStates::kDefaultPriority)
  , m_isDone(false) { }

entities::TaskValue TaskEntity::toValue() const {
  return entities::TaskValue::create(
        id,
        m_name,
        m_priority,
        m_isDone);
}

}  // namespace

namespace entities {

TaskValue::TaskValue(const TaskValue& v)
  : id(v.id), m_name(v.m_name)
  , m_priority(v.m_priority), done(v.done) { }


TaskValue& TaskValue::operator=(const TaskValue& v) {
  TaskValue tmp(v);
  //std::swap(*this, tmp);  // failed on shared ptr - segfault

  std::swap(tmp.id, id);
  m_name = tmp.m_name;  // swap doesn't work, work now
  std::swap(tmp.m_priority, m_priority);
  std::swap(tmp.done, done);

  return *this;
}

TaskValue::TaskValue(const int _id, const std::string& d, const int p, const bool _d)
  : id(_id)
  , m_name(d)
  , m_priority(p)
  , done(_d)
{ }

TaskValue TaskValue::create(const std::string& d, const int p) {
  return TaskValue(entities::EntityStates::kInactiveKey, d, p, false);
}

TaskValue TaskValue::create(const int id, const std::string& d, const int p) {
  return TaskValue(id, d, p, false);
}

TaskValue TaskValue::create() {
  auto p = entities::EntityStates::kDefaultPriority;
  return TaskValue(entities::EntityStates::kInactiveKey, std::string(), p, false);
}

TaskValue TaskValue::create(
        const int id,
        const std::string& d,
        const int p,
        const bool _done) { return TaskValue(id, d, p, _done); }
}


