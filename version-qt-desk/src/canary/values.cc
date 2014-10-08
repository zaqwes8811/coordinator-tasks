#include "top/config.h"

#include "canary/entities_and_values.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "values.h"

namespace values {

ImmutableTask::ImmutableTask(const ImmutableTask& v)
  : _id(v.id()), _description(v.description())
  , _priority(v.priority()), _done(v.done()) { }


ImmutableTask& ImmutableTask::operator=(const ImmutableTask& v) {
  ImmutableTask tmp(v);
  //std::swap(*this, tmp);  // failed on shared ptr - segfault

  std::swap(tmp._id, _id);
  std::swap(tmp._description, _description);  // swap doesn't work, work now
  std::swap(tmp._priority, _priority);
  std::swap(tmp._done, _done);

  return *this;
}

ImmutableTask::ImmutableTask(const int _id, const std::string& d, const int p, const bool _d)
  : _id(_id)
  , _description(boost::make_shared<std::string>(d))
  , _priority(p)
  , _done(_d)
{ }

ImmutableTask ImmutableTask::create(const std::string& d, const int p) {
  return ImmutableTask(entities::EntitiesStates::kInActiveKey, d, p, false);
}

ImmutableTask ImmutableTask::create(const int id, const std::string& d, const int p) {
  return ImmutableTask(id, d, p, false);
}

ImmutableTask ImmutableTask::create() {
  int p = entities::EntitiesStates::kDefaultPriority;
  return ImmutableTask(entities::EntitiesStates::kInActiveKey, std::string(), p, false);
}

ImmutableTask ImmutableTask::create(
        const int id,
        const std::string& d,
        const int p,
        const bool _done) { return ImmutableTask(id, d, p, _done); }

int ImmutableTask::id() const
{ return _id; }

boost::shared_ptr<const std::string> ImmutableTask::description() const
{ return _description; }

int ImmutableTask::priority() const
{ return _priority; }

bool ImmutableTask::done() const
{ return _done; }
}

