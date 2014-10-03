#ifndef BUSI_H
#define BUSI_H

#include "canary/entities_and_values.h"
#include "canary/pq_queries.h"
#include "canary/renders.h"
#include "canary/model.h"
#include "canary/isolation.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>

namespace models
{

class Filter {
public:
  virtual ~Filter() { }
  virtual entities::Tasks operator()(entities::Tasks t) = 0;
};

// FIXME: как вообще работать с кешем и базами данных.
//   в кешах ограниченное api!
//  http://en.wikipedia.org/wiki/Database_caching - есть про когерентность.
//
// But it need really?
//   http://stackoverflow.com/questions/548301/what-is-caching
// http://stackoverflow.com/questions/2916645/implementing-model-level-caching?rq=1
// http://stackoverflow.com/questions/343899/how-to-cache-data-in-a-mvc-application?rq=1
//   domain::TasksMirror store_cache_;
//   bool miss_;  // кеш устарел
//
// FIXME: логичекая проблема с фильтрами - как быть, если каждый раз не перезагужать кеш?
//   похоже есть зависимость от текущего фильтра
//
class Model
   : boost::noncopyable {

  void notify();  // пока пусть побудет закрытой
public:
  /// create and destory
  static Model* createInHeap(boost::shared_ptr<pq_dal::PQConnectionPool>);
  Model(entities::Tasks _model,
            boost::shared_ptr<pq_dal::PQConnectionPool> _pool);
  ~Model();

  // other
  // FIXME: да, лучше передать в конструкторе, но при конструировании возникает цикл.
  void set_listener(boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> iso);

  // наверное лучше сразу сохранить
  // добавлять все равно буду скорее всего по-одному
  void append(entities::Tasks::value_type e);
  void append_value(values::TaskValue e);  // overloading trouble in for_each

  // элемент был сохранен и есть в mirror
  void update(entities::Tasks::value_type e);

  // Жесткая привязка к списку
  entities::Tasks::value_type get_elem_by_pos(const int pos);

  void clear_store();

  // persist filters:
  //void load_all();
  static entities::Tasks load_active(const std::string& table_name,
                                     boost::shared_ptr<pq_dal::PQConnectionPool> pool);

  // render filters:
  void stable_sort_decrease_priority();

  // FIXME: плохо что хендлы утекают, и из-за того что указатели
  //   shared объекты превращаются в глобальные переменные.
  entities::Tasks get_current_model_data();

private:
  template <typename U>
  friend void renders::render_task_store(std::ostream& o, const U& a);
  void draw_task_store(std::ostream& o) const;

  std::string tasks_table_name_;
  entities::Tasks tasks_;  //

  // FIXME: кажется двойное лучше, или хранить фильтр? и через него при прорисовке пропускать?

  boost::shared_ptr<pq_dal::PQConnectionPool> pool_;
  boost::shared_ptr< ::isolation::ModelListenerMediatorDynPolym> observers_;
};
}

#endif // BUSI_H
