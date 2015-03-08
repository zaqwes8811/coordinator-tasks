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

Model* Model::createForOwn(
    boost::shared_ptr<storages::ConnectionsPool> pool)
  {
  // FIXME: дублирование. как быть с именем таблицы?
  // create tables
  auto q = pool->createTaskTableQueries(models::kTaskTableNameRef);
  q->createIfNotExist();

  auto t = load_all(models::kTaskTableNameRef, pool);
  return new Model(t, pool);
}

void Model::draw_task_store(std::ostream& o) const {
  auto q = pool_->createTaskTableQueries(tasks_table_name_);
  q->draw(o);
}

Model::~Model() { }

void Model::clear_store() {
  auto q = pool_->createTaskTableQueries(tasks_table_name_);
  q->drop();
}

Tasks Model::load_all(const std::string& table_name,
                         boost::shared_ptr<storages::ConnectionsPool> pool) {
  auto q_live = pool->createTaskLifetimeQueries(table_name);
  return Tasks(q_live->get_all());
}

entities::Tasks::value_type Model::_get_elem_by_id(const int id) {
  auto it = std::find_if(tasks_.begin(), tasks_.end()
                                    , filters::get_check_contained(id));
  assert(it != tasks_.end());
  return *it;
}

void Model::update(const values::ImmutableTask& e) {
  auto k = _get_elem_by_id(e.id());
  k->assign(e);

  auto q = pool_->createTaskLifetimeQueries(tasks_table_name_);
  q->update(k->make_value());

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
  auto q = pool_->createTaskLifetimeQueries(tasks_table_name_);

  // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
  ImmutableTask r = q->create(e->make_value());
  e->set_primary_key(r.id());  // а ведь придется оставить!!

  notify();
  _.Dismiss();
}

Model::Model(entities::Tasks _tasks,
             boost::shared_ptr<storages::ConnectionsPool> _pool)
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
