#include "top/config.h"

#include "canary/entities_and_values.h"
#include "dal/pq_queries.h"

#include <boost/make_shared.hpp>

#include <vector>


namespace entities {
using namespace pq_dal;

boost::shared_ptr<TaskEntity> TaskEntity::create(const std::string& task_name) {
  boost::shared_ptr<TaskEntity> tmp = boost::make_shared<TaskEntity>(TaskEntity());
  tmp->task_name_ = task_name;

  //const boost::shared_ptr<const TaskEntity> tmp_ = tmp;

  return tmp;
}

boost::shared_ptr<TaskEntity> TaskEntity::create(const values::ImmutableTask& v)
{
  boost::shared_ptr<TaskEntity> tmp = TaskEntity::create(*v.description());
  tmp->set_primary_key(v.id());
  tmp->set_priority(v.priority());
  tmp->set_is_done(v.done());
  return tmp;
}

void TaskEntity::assign(const values::ImmutableTask& v) {
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

void TaskEntity::set_priority(const int val)
{ priority_ = val; }

bool TaskEntity::get_is_done() const
{ return is_done_; }

// лучше так чем городить условную логику
void TaskEntity::set_is_done(bool val)
{ is_done_ = val; }

void TaskEntity::set_primary_key(int val)
{ id_ = val; }

values::ImmutableTask TaskEntity::make_value() const {
  return values::ImmutableTask::create(
        get_primary_key(),
        get_task_name(),
        get_priority(),
        get_is_done());
}

}  // namespace
