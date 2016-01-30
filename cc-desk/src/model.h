#ifndef BUSI_H
#define BUSI_H

#include "entities.h"
#include "renders.h"
#include "isolation.h"
#include "concepts.h"
#include "filters.h"

#include <actors_and_workers/actors_cc11.h>

#include <string>
#include <functional>

namespace actors {
  class UIActor;
}

namespace models
{
class Model;
namespace ext {
void onNew(gc::SharedPtr<Model> m, entities::TaskEntity e, entities::Task t);
}
/**
  \fixme как вообще работать с кешем и базами данных.
  в кешах ограниченное api!
  http://en.wikipedia.org/wiki/Database_caching - есть про когерентность.

  But it need really?
  http://stackoverflow.com/questions/548301/what-is-caching
  http://stackoverflow.com/questions/2916645/implementing-model-level-caching?rq=1
  http://stackoverflow.com/questions/343899/how-to-cache-data-in-a-mvc-application?rq=1
  domain::TasksMirror store_cache_;
  bool miss_;  // кеш устарел

  FIXME: логичекая проблема с фильтрами - как быть, если каждый раз не перезагужать кеш?
  похоже есть зависимость от текущего фильтра. А если отред. и теперь в фильтр не попадает?

  FIXME: утекают хендлы!! make ImmutableTask. причем утекают как на нижние уровни, так и на верхние

  \design
  Looks like it's bad reload full model. Need work with RAM.
  It's hard recreate state. And big overhead.
*/
class Model //: public boost::noncopyable
    // FIXME: make emb ref counter
    : public std::enable_shared_from_this<Model>
{
public:
  /// create/destory
  explicit Model(concepts::db_manager_concept_t _pool);

  /// other
  /**
    FIXME: да, лучше передать в конструкторе, но при конструировании возникает цикл.
  */
  void SetObserver(isolation::ModelListenerPtr iso);
  void addFilter(filters::FilterPtr f);
  void removeFilter(filters::FilterPtr f);

  /// Consistency space
  /**
    \design наверное лучше сразу сохранить, добавлять все равно буду скорее всего по-одному
  */
  void AppendNewTask(const entities::Task& e);

  /**
    \pre Element was persist
  */
  void UpdateTask(const entities::Task& e);
  void initialize(std::function<void(std::string)> errorHandler);
  void dropStore();

  void OnLoaded(entities::TaskEntities tasks);

public:  // lock troubles
  // typedefs
  using TaskCell = std::pair<bool, entities::TaskEntity>;

private:

  template <typename U>
  friend void renders::render_task_store(std::ostream& o, const U& a);

  friend void models::ext::onNew(gc::SharedPtr<Model> m, entities::TaskEntity e, entities::Task t);

  /**
    FIXME: плохо что хендлы утекают, и из-за того что указатели
      shared объекты превращаются в глобальные переменные.
  */
  entities::TaskEntities FilterModelData();

  /**
    Нужно было открыть для обновления при семене фильтров
  */
  void NotifyObservers();

  /**
    FIXME: кажется двойное лучше, или хранить фильтр?
      и через него при прорисовке пропускать?
  */
  gc::SharedPtr<concepts::db_manager_concept_t> m_db;
  isolation::ModelListenerPtr m_observer_ptr;

  TaskCell GetCachedTaskById(const size_t id);

  /**
    \fixme DANGER!! при реализации фильтров сломает логику!!!
      Жесткая привязка к списку и к цепочке фильтров
  */
  filters::ChainFilters m_filters_chain;

  /// Consistency Guards
  bool m_fsmNonConsistent{false};
  // FIXME: may be add state to elem - cons/incons?
  // FIXME: may be keep sorted
  //entities::TaskEntities m_tasksCache;
  // f/s is cons.?/handler
  // FIXME: trouble - extra space
  std::vector<TaskCell> m_task_cells;

public:
  // Thread safe. Redirect call to UI thread
  void RaiseErrorMessage(const std::string& message);
};
}

#endif // BUSI_H
