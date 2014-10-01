#ifndef BUSI_H
#define BUSI_H

#include "canary/entities.h"
#include "canary/pq_queries.h"
#include "canary/renders.h"
#include "canary/model.h"
#include "canary/isolation.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>

namespace app_core
{

// FIXME: закончить реализацию
// Есть одно но. Внутри нет быстрого поиска по id.
//   можно сделать хэш таблицей, и наверное это правильно, т.к.
//   это работает как кеш.
class TaskValue {
public:
    static TaskValue create(const std::string& d, const int p) {
      return TaskValue(domain::EntitiesStates::kInActiveKey, d, p);
    }

    static TaskValue create() {
      int p = domain::EntitiesStates::kDefaulPriority;
      return TaskValue(domain::EntitiesStates::kInActiveKey, std::string(), p);
    }

    const int id;
    //const std::string description;
    const int priority;
    //const bool done;  // need store

    // Придает семантику значений
    // Ухудшает локальность кеша
    boost::shared_ptr<const std::string> description;

private:
    TaskValue(const int _id, const std::string& d, const int p)
        : id(_id)
        , priority(p)
        , description(new std::string(d)) { }
};

class Model
   : boost::noncopyable {

  void notify();  // пока пусть побудет закрытой
public:
    Model(domain::TasksMirror _model,
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

  static Model* createInHeap(boost::shared_ptr<pq_dal::PQConnectionPool>);

  ~Model();

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

  // FIXME: как вообще работать с кешем и базами данных.
  //   в кешах ограниченное api!
  //  http://en.wikipedia.org/wiki/Database_caching - есть про когерентность.
  //
  // But it need really?
  //   http://stackoverflow.com/questions/548301/what-is-caching
  // http://stackoverflow.com/questions/2916645/implementing-model-level-caching?rq=1
  // http://stackoverflow.com/questions/343899/how-to-cache-data-in-a-mvc-application?rq=1
  domain::TasksMirror store_cache_;
  bool miss_;  // кеш устарел

  domain::TasksMirror model_;  //

  boost::shared_ptr<pq_dal::PQConnectionPool> pool_;

  boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> iso_;
};
}

#endif // BUSI_H
