#include "top/config.h"

#include "canary/model.h"
#include "canary/filters.h"

#include <adobe/algorithm/find.hpp>
#include <loki/ScopeGuard.h>
#include <adobe/algorithm/sort.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <functional>

namespace models {
using namespace pq_dal;
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
  using values::ImmutableTask;

using std::cout;

Model* Model::createInHeap(
    boost::shared_ptr<pq_dal::PQConnectionPool> pool)
  {
  // FIXME: дублирование. как быть с именем таблицы?
  // create tables
  TaskTableQueries q(models::kTaskTableNameRef);
  q.createIfNotExist(*(pool->get()));

  Tasks t = load_all(models::kTaskTableNameRef, pool);

  // build
  return new Model(t, pool);
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

Tasks Model::load_all(const std::string& table_name,
                         boost::shared_ptr<pq_dal::PQConnectionPool> pool) {
  TaskLifetimeQueries q_live(table_name);
  return Tasks(q_live.get_all(*(pool->get())));
}

void Model::update(Tasks::value_type e) {
  assert(e->get_primary_key() != EntitiesStates::kInActiveKey);
  assert(tasks_.end()
         != adobe::find_if(tasks_, filters::get_check_contained(e->get_primary_key())));

  TaskLifetimeQueries q(tasks_table_name_);
  q.update(e->make_value(), *(pool_->get()));

  notify();  // FIXME: а нужно ли?
}

void Model::append_value(ImmutableTask e) {
  assert(e.id() == EntitiesStates::kInActiveKey);

  Tasks::value_type h(new Tasks::value_type::element_type());
  h->set_task_name(*e.description());
  h->set_priority(e.priority());
  append(h);
}


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
void Model::append(Tasks::value_type e) {
  assert(e->get_primary_key() == EntitiesStates::kInActiveKey);

  ScopeGuard _ = MakeObjGuard(tasks_, &Tasks::pop_back);

  // FIXME: а ведь порядок операций важен, и откатить проще операцию в памяти, чем в базе данных
  tasks_.push_back(e);

  // persist full container
  TaskLifetimeQueries q(tasks_table_name_);

  // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
  ImmutableTask r = q.create(e->make_value(), *(pool_->get()));
  e->set_primary_key(r.id());  // а ведь придется оставить!!

  notify();
  _.Dismiss();
}

Model::Model(entities::Tasks _tasks,
             boost::shared_ptr<pq_dal::PQConnectionPool> _pool)
    : tasks_table_name_(models::kTaskTableNameRef)
    , tasks_(_tasks)
    , pool_(_pool) {  }

void Model::notify()
{
  observers_->update();
}

void Model::stable_sort_decrease_priority() {
  adobe::stable_sort(tasks_,
      bind(std::greater<int>(),
           bind(&TaskEntity::get_priority, _1),
           bind(&TaskEntity::get_priority, _2)));

  notify();
}

entities::Tasks::value_type Model::get_elem_by_pos(const int pos) {
  assert(pos < tasks_.size());
  return tasks_.at(pos);
}

void Model::set_listener(boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> iso)
{ observers_ = iso; }

entities::Tasks Model::get_current_model_data()
{ return tasks_; }

}
