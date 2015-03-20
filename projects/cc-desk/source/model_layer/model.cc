#include "heart/config.h"

#include "model_layer/model.h"
#include "model_layer/filters.h"

#include <loki/ScopeGuard.h>

#include <iostream>
#include <functional>

namespace models {
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
using entities::Task;

using std::cout;

void Model::dropStore() {
  auto q = m_db.getTaskTableQuery();
  new_space::drop(q);
}

void Model::setUiActor(gc::SharedPtr<actors::UIActor> a)
{ m_uiActorPtr = a; }

void Model::initializeStore(std::function<void(std::string)> errorHandler) {
  auto q = m_db.getTaskTableQuery();
  auto query = m_db.getTaskLifetimeQuery();

  // Action
  new_space::registerBeanClass(q);
  auto tasks = query.loadAll();

  // Draw
  m_tasksCache = tasks;
  notify();
}

entities::TaskEntity Model::getCachedTaskById(const size_t id) {
  auto iter = std::find_if(m_tasksCache.begin(), m_tasksCache.end(), filters::is_contained(id));
  DCHECK(iter != m_tasksCache.end());
  return *iter;
}

void Model::update(const entities::Task& e) {
  auto k = getCachedTaskById(e.id);
  auto q = m_db.getTaskLifetimeQuery();

  // Action
  q.update(k->toValue());

  // .then()
  notify();  // FIXME: а нужно ли?
}

// FIXME: may be not put in RAM? After persist view will be updated
void Model::appendNewTask(const Task& task) {
  // Create task
  // Try persist
  // Update model
  // Notify

  DCHECK(task.id == EntityStates::kInactiveKey);

  auto e = task.toEntity();

  auto _ = MakeObjGuard(m_tasksCache, &TaskEntities::pop_back);

  // FIXME: а ведь порядок операций важен, и откатить проще операцию в памяти, чем в базе данных
  m_tasksCache.push_back(e);

  // persist full container
  auto query = m_db.getTaskLifetimeQuery();

  // не правильно это! нужно сохранить одну записть. Иначе это сторонний эффект!!
  auto r = query.persist(e->toValue());
  e->id = r.id;  // а ведь придется оставить!!

  notify();
  _.Dismiss();
}

Model::Model(database_app::db_manager_concept_t _pool) : m_db(_pool)
{ }

void Model::notify()
{ m_observersPtr->update(getCurrentModelData()); }

void Model::setListener(gc::SharedPtr<isolation::ModelListener> iso)
{ m_observersPtr = iso; }

entities::TaskEntities Model::getCurrentModelData()
{ return m_filtersChain(m_tasksCache); }

}
