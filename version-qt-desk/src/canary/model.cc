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
  assert(list_tasks_.end()
         != adobe::find_if(list_tasks_, filters::get_check_contained(e->get_primary_key())));

  TaskLifetimeQueries q(tasks_table_name_);
  q.update(e, *(pool_->get()));

  observers_->update();  // FIXME: а нужно ли?
}

void Model::append(TasksMirror::value_type e) {
  // даже если исключение брошено при
  //   сохранении, а элемент добавлен, то можно потом сохр.
  // но! лучше сохранить! так мы копим несохраненные данные!
  //
  // нет, лучше транзакцией и по много не сохранять.
  //   будет зависеть от производительности.
  //   но лучше работать с
  //
  // add to container
  //
  // FIXME: может лучше исключение?
  assert(e->get_primary_key() == EntitiesStates::kInActiveKey);

  ScopeGuard _ = MakeObjGuard(list_tasks_, &TasksMirror::pop_back);
  list_tasks_.push_back(e);

  // persist full container
  TaskLifetimeQueries q(tasks_table_name_);

  // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
  q.create(e, *(pool_->get()));

  notify();
  _.Dismiss();
}

Model::Model(domain::TasksMirror _model,
        boost::shared_ptr<pq_dal::PQConnectionPool> _pool//,
        )
    : tasks_table_name_(app_core::kTaskTableNameRef), list_tasks_(_model), pool_(_pool) {  }

void Model::notify()
{
  observers_->update();
}

domain::TasksMirror::value_type Model::get_elem_by_pos(const int pos)
{
    assert(pos < list_tasks_.size());
    return list_tasks_.at(pos);
}
void Model::set_listener(boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> iso)
{ observers_ = iso; }

domain::TasksMirror Model::get_current_model_data()
{ return list_tasks_; }

}
