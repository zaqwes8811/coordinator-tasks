#include "heart/config.h"

#include "model_layer/entities.h"
#include "data_access_layer/pq_queries.h"
#include "model_layer/values.h"

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

app::SharedPtr<TaskEntity> TaskEntity::create(const values::Task& v)
{
  auto tmp = TaskEntity::create(*v.description());
  tmp->m_primaryKey = (v.id());
  tmp->m_priority = (v.priority());
  tmp->m_isDone = (v.done());
  return tmp;
}

void TaskEntity::assign(const values::Task& v) {
  m_primaryKey = v.id();
  m_name = *v.description();
  m_priority = v.priority();
  m_isDone = v.done();
}

TaskEntity::TaskEntity()
  : m_primaryKey(EntityStates::kInactiveKey)
  , m_priority(EntityStates::kDefaultPriority)
  , m_isDone(false) { }

values::Task TaskEntity::toValue() const {
  return values::Task::create(
        id(),
        name(),
        priority(),
        idDone());
}

}  // namespace
