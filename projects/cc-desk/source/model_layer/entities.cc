#include "heart/config.h"

#include "model_layer/entities.h"
#include "data_access_layer/pq_queries.h"
#include "model_layer/values.h"

#include <vector>

namespace entities {
using namespace pq_dal;

const size_t EntityStates::kInActiveKey = -1;
const int EntityStates::kDefaultPriority = 0;
const bool EntityStates::kNonDone = false;

app::SharedPtr<TaskEntity> TaskEntity::create(const std::string& task_name)
{
  auto tmp = std::make_shared<TaskEntity>(TaskEntity());
  tmp->task_name_ = task_name;
  return tmp;
}

app::SharedPtr<TaskEntity> TaskEntity::create(const values::Task& v)
{
  auto tmp = TaskEntity::create(*v.description());
  tmp->setPrimaryKey(v.id());
  tmp->setPriority(v.priority());
  tmp->set_is_done(v.done());
  return tmp;
}

void TaskEntity::assign(const values::Task& v) {
  m_id = v.id();
  task_name_ = *v.description();
  priority_ = v.priority();
  m_isDone = v.done();
}

TaskEntity::TaskEntity()
  : m_id(EntityStates::kInActiveKey)
  , priority_(EntityStates::kDefaultPriority)
  , m_isDone(false) { }

int TaskEntity::getPrimaryKey() const
{ return m_id; }

// лучше по значению
std::string TaskEntity::get_task_name() const
{ return task_name_; }

void TaskEntity::set_task_name(const std::string& value)
{ task_name_ = value; }

int TaskEntity::get_priority() const
{ return priority_; }

void TaskEntity::setPriority(const int val)
{ priority_ = val; }

bool TaskEntity::getIsDone() const
{ return m_isDone; }

// лучше так чем городить условную логику
void TaskEntity::set_is_done(bool val)
{ m_isDone = val; }

void TaskEntity::setPrimaryKey(int val)
{ m_id = val; }

values::Task TaskEntity::toValue() const {
  return values::Task::create(
        getPrimaryKey(),
        get_task_name(),
        get_priority(),
        getIsDone());
}

}  // namespace
