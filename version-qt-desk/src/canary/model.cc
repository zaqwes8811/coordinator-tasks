#include "top/config.h"

#include "canary/model.h"
#include "canary/filters.h"

#include <adobe/algorithm/find.hpp>
#include <loki/ScopeGuard.h>

#include <iostream>

namespace app_core {
using namespace pq_dal;
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
  using values::TaskValue;

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
  Tasks model(q_live.get_all(*(pool->get())));

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

void Model::update(Tasks::value_type e) {
  assert(e->get_primary_key() != EntitiesStates::kInActiveKey);
  assert(tasks_.end()
         != adobe::find_if(tasks_, filters::get_check_contained(e->get_primary_key())));

  TaskLifetimeQueries q(tasks_table_name_);
  q.update(e, *(pool_->get()));

  notify();  // FIXME: а нужно ли?
}

void Model::append_value(TaskValue e) {
  assert(e.id == EntitiesStates::kInActiveKey);

  Tasks::value_type h(new Tasks::value_type::element_type());
  h->set_task_name(*e.description);
  h->set_priority(e.priority);
  append(h);
}

void Model::append(Tasks::value_type e) {
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

  ScopeGuard _ = MakeObjGuard(tasks_, &Tasks::pop_back);
  tasks_.push_back(e);

  // persist full container
  TaskLifetimeQueries q(tasks_table_name_);

  // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
  q.create(e, *(pool_->get()));

  notify();
  _.Dismiss();
}

Model::Model(entities::Tasks _tasks,
             boost::shared_ptr<pq_dal::PQConnectionPool> _pool)
    : tasks_table_name_(app_core::kTaskTableNameRef)
    , tasks_(_tasks)
    , pool_(_pool) {  }

void Model::notify()
{
  observers_->update();
}

entities::Tasks::value_type Model::get_elem_by_pos(const int pos)
{
    assert(pos < tasks_.size());
    return tasks_.at(pos);
}
void Model::set_listener(boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> iso)
{ observers_ = iso; }

entities::Tasks Model::get_current_model_data()
{ return tasks_; }

}
