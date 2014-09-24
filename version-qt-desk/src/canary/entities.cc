#include "top/config.h"

#include "canary/entities.h"
#include "canary/pq_dal.h"

#include <boost/make_shared.hpp>

#include <vector>


namespace domain {
using namespace pq_dal;

boost::shared_ptr<TaskEntity> TaskEntity::create(std::string& task_name) {
  boost::shared_ptr<TaskEntity> tmp = boost::make_shared<TaskEntity>(TaskEntity());
  tmp->task_name_ = task_name;
  return tmp;
}


AppCore* AppCore::heapCreate(
    boost::shared_ptr<lower_level::PQConnectionPool> pool)
  {
  // create tables
  TaskTableQueries q(app::kTaskTableName);
  q.createIfNotExist(*(pool->get()));

  // get tasks

  // build
  return new AppCore(pool);
}

AppCore::~AppCore() {
  if (clear) {
    TaskTableQueries q(app::kTaskTableName);
    q.drop(*(pool_->get()));
  }
}

}  // namespace
