#include "heart/config.h"

#include "things/entities.h"
#include "dal/pq_queries.h"
#include "things/values.h"

#include <vector>

namespace entities {
using namespace pq_dal;

const int EntitiesStates::kInActiveKey = -1;
const int EntitiesStates::kDefaultPriority = 0;
const bool EntitiesStates::kNonDone = false;

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
  id_ = v.id();
  task_name_ = *v.description();
  priority_ = v.priority();
  is_done_ = v.done();
}

TaskEntity::TaskEntity()
  : id_(EntitiesStates::kInActiveKey)
  , priority_(EntitiesStates::kDefaultPriority)
  , is_done_(false) { }

int TaskEntity::get_primary_key() const
{ return id_; }

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
{ return is_done_; }

// лучше так чем городить условную логику
void TaskEntity::set_is_done(bool val)
{ is_done_ = val; }

void TaskEntity::setPrimaryKey(int val)
{ id_ = val; }

values::Task TaskEntity::toValue() const {
  return values::Task::create(
        get_primary_key(),
        get_task_name(),
        get_priority(),
        getIsDone());
}

}  // namespace
