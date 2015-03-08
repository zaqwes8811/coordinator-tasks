#ifndef BUSI_H
#define BUSI_H

#include "canary/entities_and_values.h"
#include "dal/pq_queries.h"
#include "canary/renders.h"
#include "canary/model.h"
#include "canary/isolation.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <string>

namespace models
{
// FIXME: как вообще работать с кешем и базами данных.
//   в кешах ограниченное api!
//  http://en.wikipedia.org/wiki/Database_caching - есть про когерентность.
//
// But it need really?
//   http://stackoverflow.com/questions/548301/what-is-caching
//   http://stackoverflow.com/questions/2916645/implementing-model-level-caching?rq=1
//   http://stackoverflow.com/questions/343899/how-to-cache-data-in-a-mvc-application?rq=1
//   domain::TasksMirror store_cache_;
//   bool miss_;  // кеш устарел
//
// FIXME: логичекая проблема с фильтрами - как быть, если каждый раз не перезагужать кеш?
//   похоже есть зависимость от текущего фильтра. А если отред. и теперь в фильтр не попадает?
//
// FIXME: утекают хендлы!! make ImmutableTask. причем утекают как на нижние уровни, так и на верхние
class Model
   : boost::noncopyable {
  void notify();  // Нужно было открыть для обновления при семене фильтров

public:
  // create/destory
  static Model* createForOwn(pq_dal::PQConnectionPoolPtr);
  Model(entities::Tasks _model, pq_dal::PQConnectionPoolPtr _pool);
  ~Model();

  // other
  // FIXME: да, лучше передать в конструкторе, но при конструировании возникает цикл.
  void set_listener(isolation::ModelListenerPtr iso);

  // наверное лучше сразу сохранить
  // добавлять все равно буду скорее всего по-одному
  void append(const values::ImmutableTask& e);  // overloading trouble in for_each

  // Precond: элемент был сохранен
  void update(const values::ImmutableTask& e);

  // FIXME: плохо что хендлы утекают, и из-за того что указатели
  //   shared объекты превращаются в глобальные переменные.
  entities::Tasks get_current_model_data();

  void clear_store();

private:
  template <typename U>
  friend void renders::render_task_store(std::ostream& o, const U& a);
  void draw_task_store(std::ostream& o) const;

  // persist filters:
  static entities::Tasks load_all(const std::string& table_name, pq_dal::PQConnectionPoolPtr pool);

  std::string tasks_table_name_;
  entities::Tasks tasks_;  //

  // FIXME: кажется двойное лучше, или хранить фильтр? и через него при прорисовке пропускать?
  pq_dal::PQConnectionPoolPtr pool_;
  isolation::ModelListenerPtr observers_;
  entities::Tasks::value_type _get_elem_by_id(const int id);
};
}

#endif // BUSI_H
