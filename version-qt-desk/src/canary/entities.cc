#include "top/config.h"

#include "canary/entities.h"
#include "canary/pq_dal.h"

#include <boost/make_shared.hpp>

#include <vector>


namespace domain {
using namespace pq_dal;

boost::shared_ptr<TaskEntity> TaskEntity::create(const std::string& task_name) {
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
  TaskLifetimeQueries q_live(app::kTaskTableName);
  Model model(q_live.get_all(*(pool->get())));

  // build
  return new AppCore(model, pool);
}

AppCore::~AppCore() {
  if (clear) {
    TaskTableQueries q(app::kTaskTableName);
    q.drop(*(pool_->get()));
  }
}

}  // namespace
