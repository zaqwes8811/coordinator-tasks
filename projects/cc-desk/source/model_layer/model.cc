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

#include "model_layer/model.h"
#include "model_layer/filters.h"
#include "model_layer/values.h"

#include <loki/ScopeGuard.h>

#include <iostream>
#include <functional>

namespace models {
using namespace pq_dal;
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
using values::Task;

using std::cout;

Model* Model::createForOwn(app::SharedPtr<storages::DataBaseDriver> pool) {
  // FIXME: дублирование. как быть с именем таблицы?
  // create tables
  auto q = pool->createTaskTableQuery();
  q->createIfNotExist();

  auto t = load_all(pool);
  return new Model(t, pool);
}

void Model::draw_task_store(std::ostream& o) const {
  auto q = m_dbPtr->createTaskTableQuery();
  q->draw(o);
}

Model::~Model() { }

void Model::clear_store() {
  auto q = m_dbPtr->createTaskTableQuery();
  q->drop();
}

Tasks Model::load_all(storages::DataBaseDriverPtr pool) {
  auto query = pool->createTaskLifetimeQuery();
  return Tasks(query->get_all());
}

entities::Tasks::value_type Model::getElemById(const int id) {
  auto it = std::find_if(m_tasks.begin(), m_tasks.end(), filters::is_contained(id));
  DCHECK(it != m_tasks.end());
  return *it;
}

void Model::update(const values::Task& e) {
  auto k = getElemById(e.id());
  k->assign(e);

  auto q = m_dbPtr->createTaskLifetimeQuery();
  q->update(k->toValue());

  notify();  // FIXME: а нужно ли?
}

void Model::appendNewTask(const Task& task) {
  DCHECK(task.id() == EntityStates::kInactiveKey);

  auto e = TaskEntityPtr(new TaskEntity());
  e->assign(task);

  auto _ = MakeObjGuard(m_tasks, &Tasks::pop_back);

  // FIXME: а ведь порядок операций важен, и откатить проще операцию в памяти, чем в базе данных
  m_tasks.push_back(e);

  // persist full container
  auto query = m_dbPtr->createTaskLifetimeQuery();

  // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
  auto r = query->create(e->toValue());
  e->m_primaryKey = (r.id());  // а ведь придется оставить!!

  notify();
  _.Dismiss();
}

Model::Model(entities::Tasks _tasks,
             app::SharedPtr<storages::DataBaseDriver> _pool)
    : m_tasks(_tasks)
    , m_dbPtr(_pool) {  }

void Model::notify()
{
  m_observers->update();
}

void Model::set_listener(app::SharedPtr< ::isolation::ModelListener_virtual> iso)
{ m_observers = iso; }

entities::Tasks Model::getCurrentModelData()
{ return m_tasks; }

}
