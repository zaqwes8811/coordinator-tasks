#include "heart/config.h"

#include "model_layer/entities.h"
#include "data_access_layer/postgresql_queries.h"

#include <vector>

namespace entities {
const size_t EntityStates::kInactiveKey = -1;
const int EntityStates::kDefaultPriority = 0;
const bool EntityStates::kDefaultDone = false;

TaskEntity Task::share() const {
  // FIXME: std::move - troubles with args
  // FIXME: strange but compiled
  return std::make_shared<Task>(*this);
}

Task::Task()
  : id(EntityStates::kInactiveKey)
  , priority(EntityStates::kDefaultPriority)
  , done(EntityStates::kDefaultDone) { }

entities::Task Task::toValue() const {
  Task t;
  t.id = id;
  t.name = name;
  t.priority = priority;
  t.done = done;
  return t;
}

Task::Task(const Task& v) : id(v.id), name(v.name), priority(v.priority), done(v.done)
{ }

}


