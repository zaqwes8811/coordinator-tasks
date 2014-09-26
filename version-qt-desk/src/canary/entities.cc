#include "top/config.h"

#include "canary/entities.h"
#include "canary/pq_queries.h"

#include <boost/make_shared.hpp>

#include <vector>


namespace domain {
using namespace pq_dal;

boost::shared_ptr<TaskEntity> TaskEntity::create(const std::string& task_name) {
  boost::shared_ptr<TaskEntity> tmp = boost::make_shared<TaskEntity>(TaskEntity());
  tmp->task_name_ = task_name;
  return tmp;
}
}  // namespace
