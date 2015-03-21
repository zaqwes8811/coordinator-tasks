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

using std::begin;
using std::end;
using std::cout;

void Model::dropStore() {
  auto q = m_db.getTaskTableQuery();

  // Action
  concepts::drop(q);
}

void Model::setUiActor(gc::SharedPtr<actors::UIActor> a)
{ m_uiActorPtr = a; }

void Model::initialize(std::function<void(std::string)> errorHandler) {
  // Prepare
  auto q = m_db.getTaskTableQuery();
  auto query = m_db.getTaskLifetimeQuery();

  // Action
  concepts::registerBeanClass(q);
  auto tasks = query.loadAll();

  // Important
  m_taskCells.clear();
  for (auto& task : tasks)
    m_taskCells.emplace_back(true, task);

  notifyObservers();
}

Model::TaskCell Model::getCachedTaskById(const size_t id) {
  auto iter = std::find_if(
        m_taskCells.begin(), m_taskCells.end(),
        [id] (const TaskCell& elem) -> bool {
          return filters::is_contained(id)(elem.second);
        });

  DCHECK(iter != m_tasksCache.end());
  return *iter;
}

void Model::updateTask(const entities::Task& e) {
  auto k = getCachedTaskById(e.id);

  // Is locked?
  if (!k.first) {
    return;
  } else {
    // Prepare
    auto q = m_db.getTaskLifetimeQuery();

    // Action
    q.update(k.second->toValue());

    // .then()
    notifyObservers();
  }
}

void Model::addFilter(filters::FilterPtr f)
{ m_filtersChain.add(f); notifyObservers(); }

void Model::removeFilter(filters::FilterPtr f)
{ m_filtersChain.remove(f); notifyObservers(); }

// FIXME: may be not put in RAM? After persist view will be updated
//} catch (...) {
  // No way! Can add some task after
  //auto rollback = [this]() {
    //std::remove()
    // FIXME: No can't. Lost user input!
    //this->m_taskCells.pop_back();  // no way!
  //};
  //rollback();
//}
void Model::appendNewTask(const Task& task) {
  // FIXME: show error message
  DCHECK(task.id == EntityStates::kInactiveKey);

  // RAM
  auto e = task.toEntity();
  m_taskCells.push_back({false, e});  // on lock

  // Prepare
  auto query = m_db.getTaskLifetimeQuery();

  // FIXME: May be shared from this and weak_ptr?
  auto successCallback = [e, this] (Task t) {
    *e = t;
    // std::find_if()  // off lock
    notifyObservers();
  };

  {
    // DB Actor
    auto t = query.persist(task);  // if somewhere failed - then.. state is protect?

    // UI Actor
    successCallback(t);
  }
}

Model::Model(concepts::db_manager_concept_t _pool) : m_db(_pool)
{ }

void Model::notifyObservers() {
  m_observersPtr->update(filterModelData());
}

void Model::setListener(gc::SharedPtr<isolation::ModelListener> iso) {
  m_observersPtr = iso;
}

entities::TaskEntities Model::filterModelData() {
  auto r = entities::TaskEntities();
  std::transform(begin(m_taskCells), end(m_taskCells), std::back_inserter(r),
                 [](TaskCell cell) -> entities::TaskEntity {
                    return cell.second;
                 });
  return m_filtersChain(r);
}

}
