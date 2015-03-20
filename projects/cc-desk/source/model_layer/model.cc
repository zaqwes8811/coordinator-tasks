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

  // Action
  new_space::drop(q);
}

void Model::setUiActor(gc::SharedPtr<actors::UIActor> a)
{ m_uiActorPtr = a; }

void Model::initializeStore(std::function<void(std::string)> errorHandler) {
  // Prepare
  auto q = m_db.getTaskTableQuery();
  auto query = m_db.getTaskLifetimeQuery();

  // Action
  new_space::registerBeanClass(q);
  auto tasks = query.loadAll();

  // Draw
  m_tasksCache = tasks;
  notifyObservers();
}

entities::TaskEntity Model::getCachedTaskById(const size_t id) {
  auto iter = std::find_if(
        m_tasksCache.begin(), m_tasksCache.end(),
        filters::is_contained(id));

  DCHECK(iter != m_tasksCache.end());
  return *iter;
}

void Model::updateTask(const entities::Task& e) {
  auto k = getCachedTaskById(e.id);

  // Prepare
  auto q = m_db.getTaskLifetimeQuery();

  // Action
  q.update(k->toValue());

  // .then()
  notifyObservers();
}

void Model::addFilter(filters::FilterPtr f)
{ m_filtersChain.add(f); notifyObservers(); }

void Model::removeFilter(filters::FilterPtr f)
{ m_filtersChain.remove(f); notifyObservers(); }

// FIXME: may be not put in RAM? After persist view will be updated
void Model::appendNewTask(const Task& task) {
  DCHECK(task.id == EntityStates::kInactiveKey);

  auto e = task.toEntity();
  m_tasksCache.push_back(e);

  // Prepare
  auto query = m_db.getTaskLifetimeQuery();

  // Action
  try {
    *e = query.persist(task);
  } catch (...) {
    // No way! Can add some task after
    auto rollback = [this]() {
      //std::remove()
      this->m_tasksCache.pop_back();
    };
    rollback();
  }

  // .then()
  notifyObservers();
}

Model::Model(database_app::db_manager_concept_t _pool) : m_db(_pool)
{ }

void Model::notifyObservers() {
  m_observersPtr->update(filterModelData());
}

void Model::setListener(gc::SharedPtr<isolation::ModelListener> iso) {
  m_observersPtr = iso;
}

entities::TaskEntities Model::filterModelData() {
  return m_filtersChain(m_tasksCache);
}

}
