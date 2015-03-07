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

#include "top/config.h"

#include "canary/model.h"
#include "canary/filters.h"

#include <loki/ScopeGuard.h>
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
  TaskTableQueries q(models::kTaskTableNameRef, &(*(pool->get())));
  q.createIfNotExist();

  Tasks t = load_all(models::kTaskTableNameRef, pool);
  return new Model(t, pool);
}

void Model::draw_task_store(std::ostream& o) const {
  TaskTableQueries q(tasks_table_name_, &(*(pool_->get())));
  q.print(o);
}

Model::~Model() { }

void Model::clear_store() {
  TaskTableQueries q(tasks_table_name_, &(*(pool_->get())));
  q.drop();
}

Tasks Model::load_all(const std::string& table_name,
                         boost::shared_ptr<pq_dal::PQConnectionPool> pool) {
  TaskLifetimeQueries q_live(table_name);
  return Tasks(q_live.get_all(*(pool->get())));
}

entities::Tasks::value_type Model::_get_elem_by_id(const int id) {
  Tasks::iterator it = std::find_if(tasks_.begin(), tasks_.end()
                                    , filters::get_check_contained(id));
  assert(it != tasks_.end());
  return *it;
}

void Model::update(const values::ImmutableTask& e) {
  Tasks::value_type k = _get_elem_by_id(e.id());
  k->assign(e);

  TaskLifetimeQueries q(tasks_table_name_);
  q.update(k->make_value(), *(pool_->get()));

  notify();  // FIXME: а нужно ли?
}

void Model::append(const ImmutableTask& v) {
  assert(v.id() == EntitiesStates::kInActiveKey);

  Tasks::value_type e(new Tasks::value_type::element_type());
  e->assign(v);

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

void Model::set_listener(boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> iso)
{ observers_ = iso; }

entities::Tasks Model::get_current_model_data()
{ return tasks_; }

}
