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

#include "heart/config.h"

#include "canary/model.h"
#include "canary/filters.h"
#include "things/values.h"

#include <loki/ScopeGuard.h>

#include <iostream>
#include <functional>

namespace models {
using namespace pq_dal;
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
using values::ImmutableTask;

using std::cout;

Model* Model::createForOwn(app::SharedPtr<storages::DataBaseDriver> pool) {
  // FIXME: дублирование. как быть с именем таблицы?
  // create tables
  auto q = pool->createTaskTableQueries();
  q->createIfNotExist();

  auto t = load_all(pool);
  return new Model(t, pool);
}

void Model::draw_task_store(std::ostream& o) const {
  auto q = m_db_ptr->createTaskTableQueries();
  q->draw(o);
}

Model::~Model() { }

void Model::clear_store() {
  auto q = m_db_ptr->createTaskTableQueries();
  q->drop();
}

Tasks Model::load_all(storages::DataBaseDriverPtr pool) {
  auto q_live = pool->createTaskLifetimeQueries();
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

  auto q = m_db_ptr->createTaskLifetimeQueries();
  q->update(k->make_value());

  notify();  // FIXME: а нужно ли?
}

void Model::append(const ImmutableTask& v) {
  assert(v.id() == EntitiesStates::kInActiveKey);

  Tasks::value_type e(new Tasks::value_type::element_type());
  e->assign(v);

  auto _ = MakeObjGuard(tasks_, &Tasks::pop_back);

  // FIXME: а ведь порядок операций важен, и откатить проще операцию в памяти, чем в базе данных
  tasks_.push_back(e);

  // persist full container
  auto q = m_db_ptr->createTaskLifetimeQueries();

  // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
  auto r = q->create(e->make_value());
  e->set_primary_key(r.id());  // а ведь придется оставить!!

  notify();
  _.Dismiss();
}

Model::Model(entities::Tasks _tasks,
             app::SharedPtr<storages::DataBaseDriver> _pool)
    : tasks_(_tasks)
    , m_db_ptr(_pool) {  }

void Model::notify()
{
  m_observers->update();
}

void Model::set_listener(app::SharedPtr< ::isolation::ModelListenerMediatorDynPolym> iso)
{ m_observers = iso; }

entities::Tasks Model::getCurrentModelData()
{ return tasks_; }

}
