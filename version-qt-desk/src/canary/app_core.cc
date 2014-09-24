#include "top/config.h"

#include "canary/app_core.h"

namespace busi {
using namespace pq_dal;
using namespace domain;

AppCore* AppCore::heapCreate(
    boost::shared_ptr<pq_dal::PQConnectionPool> pool)
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
}
