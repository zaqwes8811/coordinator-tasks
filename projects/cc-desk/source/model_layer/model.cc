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
using std::find_if;
using namespace std::placeholders;
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
void onNew(gc::SharedPtr<Model> m, TaskEntity task_ptr, Task saved_task)
{
  *task_ptr = saved_task;
  auto iter =
      std::find_if(begin(m->m_task_cells), end(m->m_task_cells),
                   [task_ptr] (models::Model::TaskCell v) { return task_ptr->id == v.second->id; });

  if (iter == end(m->m_task_cells)) return;

  unlock(*iter);
  m->NotifyObservers();
}
}  // space

void Model::dropStore() {
  auto db = m_db;

  Dispatcher::Post(Dispatcher::DB, [db]() {
    auto q = db->getTaskTableQuery();
    concepts::drop(q);
  });
}

void Model::initialize(std::function<void(std::string)>)
{
  // FIXME: to method or free function
  auto on_loaded = [this](entities::TaskEntities tasks) {
    m_task_cells.clear();
    for (auto& task : tasks)
      m_task_cells.emplace_back(true, task);
    NotifyObservers();
  };

  auto db = m_db;

  Dispatcher::Post(Dispatcher::DB, [on_loaded, db] {
    auto tables = std::vector<concepts::table_concept_t>{
        db->getTaskTableQuery(),
        db->getTagTableQuery()};

    for (auto& table : tables)
      concepts::registerBeanClass(table);

    auto tasks = db->getTaskLifetimeQuery().loadAll();

    Dispatcher::Post(Dispatcher::UI, std::bind(on_loaded, tasks));
  });
}

Model::TaskCell Model::GetCachedTaskById(const size_t id)
{
  auto it = std::find_if(
        m_task_cells.begin(), m_task_cells.end(),
        [id] (const TaskCell& elem) -> bool { return filters::is_contained(id)(elem.second); });

  // FIXME: maybe make real check?
  DCHECK(it != end(m_task_cells));
  return *it;
}

void Model::updateTask(const entities::Task& updated_task)
{
  auto old_cell = GetCachedTaskById(updated_task.id);

  if (!try_lock(old_cell)) {
    return;
  } else {
    lock(old_cell);

    auto on_update = [this, old_cell] (const entities::Task& task) mutable {
      *(old_cell.second) = task;
      unlock(old_cell);
      NotifyObservers();
    };

    auto db = m_db;
    Dispatcher::Post(Dispatcher::DB, [on_update, db, updated_task] {
      db->getTaskLifetimeQuery().update(updated_task);
      Dispatcher::Post(Dispatcher::UI, bind(on_update, updated_task));
    });
  }
}

void Model::addFilter(filters::FilterPtr f)
{ m_filters_chain.add(f); NotifyObservers(); }

void Model::removeFilter(filters::FilterPtr f)
{ m_filters_chain.remove(f); NotifyObservers(); }

void Model::appendNewTask(const Task& unsaved_task)
{
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

entities::TaskEntities Model::FilterModelData()
{
  auto r = entities::TaskEntities();
  std::transform(begin(m_task_cells), end(m_task_cells),
                 std::back_inserter(r),
                 [](TaskCell cell) -> entities::TaskEntity { return cell.second;});

  return m_filters_chain(r);
}

Model::Model(concepts::db_manager_concept_t _pool)
  : m_db(std::make_shared<concepts::db_manager_concept_t>(_pool))
{ }

void Model::NotifyObservers()
{ m_observer_ptr->update(FilterModelData()); }

void Model::SetObserver(gc::SharedPtr<isolation::ModelListener> observer)
{ m_observer_ptr = observer; }

void Model::RaiseErrorMessage(const std::string& message)
{
  auto view = m_observer_ptr;  // FIXME: thread-safe but... don't like it
  Dispatcher::Post(Dispatcher::UI,
                   [view, message] { view->DrawErrorMessage(message); }
  );
}
}
