#include "top/config.h"

#include "canary/app_core.h"
#include <loki/ScopeGuard.h>

namespace app_core {
using namespace pq_dal;
using namespace domain;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;

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

void AppCore::draw_task_store(std::ostream& o) const {
  TaskTableQueries q(app::kTaskTableName);
  q.print(o, *(pool_->get()));
}

AppCore::~AppCore() {
  if (clear) {
    TaskTableQueries q(app::kTaskTableName);
    q.drop(*(pool_->get()));
  }
}

void AppCore::append(Model::value_type e) {
  // FIXME: может лучше исключение?
  assert(e->get_primary_key() == EntitiesStates::kInActiveKey);

  try {
    // даже если исключение брошено при
    //   сохранении, а элемент добавлен, то можно потом сохр.
    // но! лучше сохранить! так мы копим несохраненные данные!
    //
    // нет, лучше транзакцией и по много не сохранять.
    //   будет зависеть от производительности.
    //   но лучше работать с
    //
    // add to container
    ScopeGuard _ = MakeObjGuard(model_, &Model::pop_back);
    model_.push_back(e);

    // persist full container
    TaskLifetimeQueries q(app::kTaskTableName);
    q.persist(model_, *(pool_->get()));

    _.Dismiss();
  } catch (...) {

    throw;
  }
}
}
