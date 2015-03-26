#include "heart/config.h"

#include "model_layer/model.h"
#include "model_layer/filters.h"
#include "core/actor_ui.h"

#include <loki/ScopeGuard.h>
#include <actors_and_workers/arch.h>

#include <iostream>
#include <functional>

namespace models {
using namespace entities;
using Loki::ScopeGuard;
using Loki::MakeObjGuard;
using entities::Task;
using std::bind;

using std::begin;
using std::end;
using std::cout;

void lock(Model::TaskCell& r_cell) {
  r_cell.first = true;
}

bool try_lock(Model::TaskCell& r_cell) {
  return r_cell.first == false;
}

void unlock(Model::TaskCell& r_cell) {
  r_cell.first = false;
}

namespace ext {
void onNew(gc::SharedPtr<Model> m, TaskEntity task_ptr, Task saved_task) {
  *task_ptr = saved_task;
  auto iter =
      std::find_if(begin(m->m_task_cells), end(m->m_task_cells),
                   [task_ptr] (models::Model::TaskCell v) { return task_ptr->id == v.second->id; });

  if (iter == end(m->m_task_cells)) return;

  unlock(*iter);
  m->notifyObservers();
}
}  // space

void Model::dropStore() {
  auto db = m_db;

  Dispatcher::Post(Dispatcher::DB, [db]() {
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

  Dispatcher::Post(Dispatcher::DB, [onLoaded, db] {
    auto tables = std::vector<concepts::table_concept_t>{db->getTaskTableQuery(), db->getTagTableQuery()};
    for (auto& table : tables)
      concepts::registerBeanClass(table);

    auto tasks = db->getTaskLifetimeQuery().loadAll();

    Dispatcher::Post(Dispatcher::UI, std::bind(onLoaded, tasks));
  });
}

Model::TaskCell Model::GetCachedTaskById(const size_t id) {
  // Check contain
  DCHECK(std::find_if(
      begin(m_task_cells), end(m_task_cells),
      [id, this] (const TaskCell& elem) -> bool { return filters::is_contained(id)(elem.second);})
         != end(m_task_cells));

  auto it = std::find_if(
        m_task_cells.begin(), m_task_cells.end(),
        [id] (const TaskCell& elem) -> bool { return filters::is_contained(id)(elem.second); });

  auto r = *it;
  return r;
}

void Model::updateTask(const entities::Task& updated_task) {
  auto old_cell = GetCachedTaskById(updated_task.id);

  // Is locked?
  if (!try_lock(old_cell)) {
    return;
  } else {
    lock(old_cell);

    // FIXME: lock - disable other updates
    auto on_update = [this, old_cell] (const entities::Task& task) mutable {
      *(old_cell.second) = task;
      unlock(old_cell);
      notifyObservers();
    };

    auto db = m_db;
    Dispatcher::Post(Dispatcher::DB, [on_update, db, updated_task] {
      db->getTaskLifetimeQuery().update(updated_task);
      Dispatcher::Post(Dispatcher::UI, bind(on_update, updated_task));
    });
  }
}

void Model::addFilter(filters::FilterPtr f)
{ m_filters_chain.add(f); notifyObservers(); }

void Model::removeFilter(filters::FilterPtr f)
{ m_filters_chain.remove(f); notifyObservers(); }

void Model::appendNewTask(const Task& unsaved_task) {
  /**
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
  */

  using std::find_if;
  using namespace std::placeholders;

  // FIXME: show error message
  DCHECK(unsaved_task.id == EntityStates::kInactiveKey);

  // on lock
  auto unsaved_task_ptr = unsaved_task.share();
  auto cell = TaskCell{false, unsaved_task_ptr};
  lock(cell);
  m_task_cells.push_back(cell);

  auto on_success = std::bind(&ext::onNew, shared_from_this(), unsaved_task_ptr, _1);

  auto db_ptr = m_db;
  Dispatcher::Post(Dispatcher::DB, [unsaved_task, on_success, db_ptr] () {
    auto saved_task = db_ptr->getTaskLifetimeQuery().persist(unsaved_task);
    Dispatcher::Post(Dispatcher::UI, std::bind(on_success, saved_task));
  });
}

entities::TaskEntities Model::filterModelData() {
  auto r = entities::TaskEntities();
  std::transform(begin(m_task_cells), end(m_task_cells),
                 std::back_inserter(r),
                 [](TaskCell cell) -> entities::TaskEntity { return cell.second;});

  return m_filters_chain(r);
}

Model::Model(concepts::db_manager_concept_t _pool)
  : m_db(std::make_shared<concepts::db_manager_concept_t>(_pool)) { }
void Model::notifyObservers() { m_observersPtr->update(filterModelData()); }
void Model::setListener(gc::SharedPtr<isolation::ModelListener> iso) { m_observersPtr = iso; }

}
