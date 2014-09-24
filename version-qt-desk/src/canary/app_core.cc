#include "top/config.h"

#include "canary/app_core.h"

namespace app_core {
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

    // persist full container

  } catch (...) {

    throw;
  }
}
}
