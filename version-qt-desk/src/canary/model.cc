#include "top/config.h"

#include "canary/model.h"
#include "canary/filters.h"

#include <adobe/algorithm/find.hpp>
#include <loki/ScopeGuard.h>

#include <iostream>

namespace app_core {
using namespace pq_dal;
using namespace domain;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;

using std::cout;

Model* Model::createInHeap(
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
  return new Model(model, pool);
}

void Model::draw_task_store(std::ostream& o) const {
  TaskTableQueries q(tasks_table_name_);
  q.print(o, *(pool_->get()));
}

Model::~Model() { }

void Model::clear_store() {
  TaskTableQueries q(tasks_table_name_);
  q.drop(*(pool_->get()));
}

void Model::update(TasksMirror::value_type e) {
  assert(e->get_primary_key() != EntitiesStates::kInActiveKey);
  assert(store_cache_.end()
         != adobe::find_if(store_cache_,
                           filters::get_check_contained(e->get_primary_key())));

  TaskLifetimeQueries q(tasks_table_name_);
  q.update(e, *(pool_->get()));

  iso_->update();  // FIXME: а нужно ли?
}

void Model::append(TasksMirror::value_type e) {
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
    ScopeGuard _ = MakeObjGuard(store_cache_, &TasksMirror::pop_back);
    ScopeGuard _m = MakeObjGuard(model_, &TasksMirror::pop_back);
    store_cache_.push_back(e);
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

Model::Model(domain::TasksMirror _model,
        boost::shared_ptr<pq_dal::PQConnectionPool> _pool//,
        )
    : tasks_table_name_(app_core::kTaskTableNameRef), store_cache_(_model), miss_(false), pool_(_pool) {  }

void Model::notify()
{
  iso_->update();
}
}
