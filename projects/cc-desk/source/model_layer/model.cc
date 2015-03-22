#include "heart/config.h"

#include "model_layer/model.h"
#include "model_layer/filters.h"
#include "core/actor_ui.h"

#include <loki/ScopeGuard.h>

#include <iostream>
#include <functional>

extern gc::SharedPtr<actors::UIActor> gUIActor;  // dtor will call and app out
extern gc::SharedPtr<cc11::Actior> gDBActor;

namespace models {
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
using entities::Task;

using std::begin;
using std::end;
using std::cout;

namespace ext {
void onNew(gc::SharedPtr<Model> m, TaskEntity task_ptr, Task saved_task) {
  *task_ptr = saved_task;
  auto iter =
      std::find_if(begin(m->m_task_cells), end(m->m_task_cells),
                   [task_ptr] (models::Model::TaskCell v) { return task_ptr->id == v.second->id; });

  if (iter == end(m->m_task_cells)) return;

  iter->first = true; // off lock
  m->notifyObservers();
}
}  // space

void Model::dropStore() {
  auto db = m_db;

  gDBActor->post([db]() {
    auto q = db->getTaskTableQuery();
    concepts::drop(q);
  });
}

//template <typename T>

void Model::initialize(std::function<void(std::string)> errorHandler) {

  auto onLoaded = [this](entities::TaskEntities tasks) {
    m_task_cells.clear();
    for (auto& task : tasks)
      m_task_cells.emplace_back(true, task);
    notifyObservers();
  };

  auto db = m_db;
  gDBActor->post([onLoaded, db] {
    auto tables = std::vector<concepts::table_concept_t>{db->getTaskTableQuery(), db->getTagTableQuery()};
    for (auto& table : tables)
      concepts::registerBeanClass(table);

    auto tasks = db->getTaskLifetimeQuery().loadAll();

    gUIActor->post(std::bind(onLoaded, tasks));
  });
}

Model::TaskCell Model::GetCachedTaskById(const size_t id) {
  // Check contain
  DCHECK(std::find_if(
      begin(m_task_cells), end(m_task_cells),
      [id] (const TaskCell& elem) -> bool { return filters::is_contained(id)(elem.second);})
         != end(m_tasks_cells));

  auto iter = std::find_if(
        m_task_cells.begin(), m_task_cells.end(),
        [id] (const TaskCell& elem) -> bool { return filters::is_contained(id)(elem.second); });

  return *iter;
}

void Model::updateTask(const entities::Task& e) {
  auto cell_to_update = GetCachedTaskById(e.id);

  // Is locked?
  if (!cell_to_update.first) {
    return;
  } else {
    auto on_update = std::bind(&Model::notifyObservers, shared_from_this());

    auto db = m_db;
    gDBActor->post([on_update, db, cell_to_update] {
      auto value = cell_to_update.second->toValue();
      db->getTaskLifetimeQuery().update(value);
      gUIActor->post(on_update);
    });
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
// FIXME: if somewhere failed after persist - then.. state is protect?
void Model::appendNewTask(const Task& unsaved_task) {
  using std::find_if;
  using namespace std::placeholders;

  // FIXME: show error message
  DCHECK(unsaved_task.id == EntityStates::kInactiveKey);

  // on lock
  auto unsaved_task_ptr = unsaved_task.share();
  m_task_cells.push_back({false, unsaved_task_ptr});

  auto unlock = std::bind(&ext::onNew, shared_from_this(), unsaved_task_ptr, _1);

  auto db_ptr = m_db;
  gDBActor->post([unsaved_task, unlock, db_ptr] () {
    auto saved_task = db_ptr->getTaskLifetimeQuery().persist(unsaved_task);
    gUIActor->post(std::bind(unlock, saved_task));
  });
}

entities::TaskEntities Model::filterModelData() {
  auto r = entities::TaskEntities();
  std::transform(begin(m_task_cells), end(m_task_cells),
                 std::back_inserter(r),
                 [](TaskCell cell) -> entities::TaskEntity { return cell.second;});

  return m_filtersChain(r);
}

Model::Model(concepts::db_manager_concept_t _pool) : m_db(std::make_shared<concepts::db_manager_concept_t>(_pool)) { }
void Model::notifyObservers() { m_observersPtr->update(filterModelData()); }
void Model::setListener(gc::SharedPtr<isolation::ModelListener> iso) { m_observersPtr = iso; }

}
