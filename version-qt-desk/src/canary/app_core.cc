#include "top/config.h"

#include "canary/app_core.h"
#include "canary/filters.h"

#include <adobe/algorithm/find.hpp>
#include <loki/ScopeGuard.h>

namespace app_core {
using namespace pq_dal;
using namespace domain;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;

AppCore* AppCore::createInHeap(
    boost::shared_ptr<pq_dal::PQConnectionPool> pool)
  {
  // FIXME: дублирование. как быть с именем таблицы?
  // create tables
  TaskTableQueries q(app_core::kTaskTableNameRef);
  q.createIfNotExist(*(pool->get()));

  // get tasks
  TaskLifetimeQueries q_live(app_core::kTaskTableNameRef);
  TasksMirror model(q_live.get_all(*(pool->get())));

  // build
  return new AppCore(model, pool);
}

void AppCore::draw_task_store(std::ostream& o) const {
  TaskTableQueries q(tasks_table_name_);
  q.print(o, *(pool_->get()));
}

AppCore::~AppCore() { }

void AppCore::clear_store() {
  TaskTableQueries q(tasks_table_name_);
  q.drop(*(pool_->get()));
}

void AppCore::update(TasksMirror::value_type e) {
  assert(e->get_primary_key() != EntitiesStates::kInActiveKey);
  assert(store_mirror_.end() != adobe::find_if(store_mirror_, filters::get_check_contained(e->get_primary_key())));

  TaskLifetimeQueries q(tasks_table_name_);
  q.update(e, *(pool_->get()));
}

void AppCore::append(TasksMirror::value_type e) {
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
    ScopeGuard _ = MakeObjGuard(store_mirror_, &TasksMirror::pop_back);
    ScopeGuard _m = MakeObjGuard(model_, &TasksMirror::pop_back);
    store_mirror_.push_back(e);
    model_.push_back(e);

    // persist full container
    TaskLifetimeQueries q(tasks_table_name_);

    // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
    q.create(e, *(pool_->get()));

    _.Dismiss();
    _m.Dismiss();

    notify();

  } catch (...) {

    throw;
  }
}

AppCore::AppCore(domain::TasksMirror _model,
        boost::shared_ptr<pq_dal::PQConnectionPool> _pool//,
        )
    : tasks_table_name_(app_core::kTaskTableNameRef), store_mirror_(_model), miss_(false), pool_(_pool) {  }

void AppCore::notify()
{
  iso_->update();
}
}
