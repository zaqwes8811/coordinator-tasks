#include "top/config.h"

#include "canary/entities_and_values.h"
#include "canary/pq_queries.h"

#include <boost/make_shared.hpp>

#include <vector>


namespace entities {
using namespace pq_dal;

boost::shared_ptr<TaskEntity> TaskEntity::create(const std::string& task_name) {
  boost::shared_ptr<TaskEntity> tmp = boost::make_shared<TaskEntity>(TaskEntity());
  tmp->task_name_ = task_name;
  return tmp;
}

TaskEntity::TaskEntity()
  : primary_key_(EntitiesStates::kInActiveKey)
  , priority_(EntitiesStates::kDefaulPriority)
  , is_done_(false) { }

int TaskEntity::get_primary_key() const
{ return primary_key_; }

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

}  // namespace
