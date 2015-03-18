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

#include <loki/ScopeGuard.h>

#include <iostream>
#include <functional>

namespace models {
using namespace pq_dal;
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
using entities::Task;

using std::cout;

Model* Model::createForOwn(gc::SharedPtr<storages::DataBase> dbPtr) {
  // FIXME: дублирование. как быть с именем таблицы?
  // create tables
  auto q = dbPtr->getTaskTableQuery();
  q->registerBeanClass();

  auto t = load_all(dbPtr);
  return new Model(t, dbPtr);
}

void Model::draw_task_store(std::ostream& o) const {
  //auto q = m_dbPtr->createTaskTableQuery();
  //q->draw(o);
}

Model::~Model() { }

void Model::dropStore() {
  auto q = m_dbPtr->getTaskTableQuery();
  m_dbPtr->dropSchema(std::move(q));
}

TaskEntities Model::load_all(storages::DataBasePtr dbPtr) {
  auto query = dbPtr->getTaskLifetimeQuery();
  return TaskEntities(query->loadAll());
}

entities::TaskEntity Model::getElemById(const size_t id) {
  auto it = std::find_if(m_tasks.begin(), m_tasks.end(), filters::is_contained(id));
  DCHECK(it != m_tasks.end());
  return *it;
}

void Model::update(const entities::Task& e) {
  auto k = getElemById(e.id);

  auto q = m_dbPtr->getTaskLifetimeQuery();
  q->update(k->toValue());

  notify();  // FIXME: а нужно ли?
}

// FIXME: may be not put in RAM? After persist view will be updated
void Model::appendNewTask(const Task& task) {
  DCHECK(task.id == EntityStates::kInactiveKey);

  auto e = Task::createEntity(task);

  auto _ = MakeObjGuard(m_tasks, &TaskEntities::pop_back);

  // FIXME: а ведь порядок операций важен, и откатить проще операцию в памяти, чем в базе данных
  m_tasks.push_back(e);

  // persist full container
  auto query = m_dbPtr->getTaskLifetimeQuery();

  // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
  auto r = query->persist(e->toValue());
  e->id = r.id;  // а ведь придется оставить!!

  notify();
  _.Dismiss();
}

Model::Model(entities::TaskEntities _tasks, gc::SharedPtr<storages::DataBase> _pool)
    : m_tasks(_tasks), m_dbPtr(_pool) {  }

void Model::notify()
{ m_observersPtr->update(); }

void Model::set_listener(gc::SharedPtr<isolation::ModelListener_virtual> iso)
{ m_observersPtr = iso; }

entities::TaskEntities Model::getCurrentModelData()
{ return m_tasks; }

}
