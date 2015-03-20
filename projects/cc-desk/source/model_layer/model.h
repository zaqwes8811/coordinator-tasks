#ifndef BUSI_H
#define BUSI_H

#include "model_layer/entities.h"
#include "view/renders.h"
#include "model_layer/isolation.h"
#include "core/concepts.h"
#include "filters.h"

#include <string>
#include <functional>

namespace actors {
  class UIActor;
}

namespace models
{
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
{
public:
  /// create/destory
  explicit Model(database_app::db_manager_concept_t _pool);

  /// other
  /**
    FIXME: да, лучше передать в конструкторе, но при конструировании возникает цикл.
  */
  void setListener(isolation::ModelListenerPtr iso);

  /**
    \fixme наверное лучше сразу сохранить, добавлять все равно буду скорее всего по-одному
  */
  void appendNewTask(const entities::Task& e);  // overloading trouble in for_each

  /**
    \pre лемент был сохранен
  */
  void update(const entities::Task& e);

  void initializeStore(std::function<void(std::string)> errorHandler);

  void dropStore();

  void setUiActor(gc::SharedPtr<actors::UIActor> a);

  void add(filters::FilterPtr f)
  { m_filtersChain.add(f); notifyObservers(); }

  void remove(filters::FilterPtr f)
  { m_filtersChain.remove(f); notifyObservers(); }

private:
  template <typename U>
  friend void renders::render_task_store(std::ostream& o, const U& a);

  /**
    FIXME: плохо что хендлы утекают, и из-за того что указатели
      shared объекты превращаются в глобальные переменные.
  */
  entities::TaskEntities getCurrentModelData();

  /**
    Нужно было открыть для обновления при семене фильтров
  */
  void notifyObservers();

  entities::TaskEntities m_tasksCache;

  /**
    FIXME: кажется двойное лучше, или хранить фильтр?
      и через него при прорисовке пропускать?
  */
  database_app::db_manager_concept_t m_db;
  isolation::ModelListenerPtr m_observersPtr;
  entities::TaskEntity getCachedTaskById(const size_t id);

  gc::WeakPtr<actors::UIActor> m_uiActorPtr;

  /**
    \fixme DANGER!! при реализации фильтров сломает логику!!!
      Жесткая привязка к списку и к цепочке фильтров
  */
  filters::ChainFilters m_filtersChain;

  /// Consistency Guards
  bool m_fsmAppendIsDone{false};
};
}

#endif // BUSI_H
