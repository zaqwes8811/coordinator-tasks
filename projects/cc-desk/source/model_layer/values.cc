#include "heart/config.h"

#include "model_layer/entities.h"

#include "values.h"

namespace values {

Task::Task(const Task& v)
  : _id(v.id()), _description(v.description())
  , _priority(v.priority()), _done(v.done()) { }


Task& Task::operator=(const Task& v) {
  Task tmp(v);
  //std::swap(*this, tmp);  // failed on shared ptr - segfault

  std::swap(tmp._id, _id);
  std::swap(tmp._description, _description);  // swap doesn't work, work now
  std::swap(tmp._priority, _priority);
  std::swap(tmp._done, _done);

  return *this;
}

Task::Task(const int _id, const std::string& d, const int p, const bool _d)
  : _id(_id)
  , _description(std::make_shared<std::string>(d))
  , _priority(p)
  , _done(_d)
{ }

Task Task::create(const std::string& d, const int p) {
  return Task(entities::EntityStates::kInactiveKey, d, p, false);
}

Task Task::create(const int id, const std::string& d, const int p) {
  return Task(id, d, p, false);
}

Task Task::create() {
  auto p = entities::EntityStates::kDefaultPriority;
  return Task(entities::EntityStates::kInactiveKey, std::string(), p, false);
}

Task Task::create(
        const int id,
        const std::string& d,
        const int p,
        const bool _done) { return Task(id, d, p, _done); }

size_t Task::id() const
{ return _id; }

app::SharedPtr<const std::string> Task::description() const
{ return _description; }

int Task::priority() const
{ return _priority; }

bool Task::done() const
{ return _done; }
}

