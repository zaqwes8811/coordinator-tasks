#ifndef BUSI_H
#define BUSI_H

#include "canary/entities.h"
#include "canary/pq_queries.h"
#include "canary/renders.h"
#include "canary/app_types.h"
#include "canary/isolation.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace app_core
{

// FIXME: закончить реализацию
class TaskValues {
public:
    TaskValues()
        : id(domain::EntitiesStates::kInActiveKey)
        , priority(domain::EntitiesStates::kDefaulPriority) {}

    const int id;
    const std::string description;
    const int priority;
};

class AppCore
   : boost::noncopyable {

  void notify();  // пока пусть побудет закрытой
public:
    AppCore(domain::TasksMirror _model,
            boost::shared_ptr<pq_dal::PQConnectionPool> _pool);

  // FIXME: да, лучше передать в конструкторе, но при конструировании возникает цикл.
  void set_listener(boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> iso)
  { iso_ = iso; }

  // наверное лучше сразу сохранить
  // добавлять все равно буду скорее всего по-одному
  void append(domain::TasksMirror::value_type e);

  // элемент был сохранен и есть в mirror
  void update(domain::TasksMirror::value_type e);

  // Жесткая привязка к списку
  domain::TasksMirror::value_type get_elem_by_pos(const int pos)
  {
      assert(pos < model_.size());
      return model_.at(pos);
  }

  //void save_all();

  static AppCore* createInHeap(boost::shared_ptr<pq_dal::PQConnectionPool>);

  ~AppCore();

  void clear_store();

  // persist filters:
  void load_all();
  void load_active();

  // render filters:

  // FIXME: плохо что хендлы утекают, и из-за того что указатели
  //   shared объекты превращаются в глобальные переменные.
  domain::TasksMirror get_current_model_data()
  { return model_; }

private:
  template <typename U>
  friend void renders::render_task_store(std::ostream& o, const U& a);

  void draw_task_store(std::ostream& o) const;

  std::string tasks_table_name_;

  domain::TasksMirror store_mirror_;
  bool miss_;  // кеш устарел

  domain::TasksMirror model_;  //

  boost::shared_ptr<pq_dal::PQConnectionPool> pool_;

  boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> iso_;
};
}

#endif // BUSI_H
