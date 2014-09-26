#ifndef BUSI_H
#define BUSI_H

#include "canary/entities.h"
#include "canary/pq_queries.h"
#include "canary/renders.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace app_core
{
class AppCore
   : boost::noncopyable {
public:
    AppCore(
            domain::TasksMirror _model,
            boost::shared_ptr<pq_dal::PQConnectionPool> _pool)
        : miss_(true), pool_(_pool) {
        model_ = (_model);  // assign
        miss_ = false;  // последние данные загружены
    }

  // наверное лучше сразу сохранить
  // добавлять все равно буду скорее всего по-одному
  void append(domain::TasksMirror::value_type e);

  // элемент был сохранен и есть в mirror
  void update(domain::TasksMirror::value_type e);

  //void save_all();

  static AppCore* heapCreate(
      boost::shared_ptr<pq_dal::PQConnectionPool>);

  ~AppCore();

  void clear_store();

  // persist filters:
  void load_all();
  void load_active();

  // render filters:

private:
  template <typename U>
  friend void renders::render_task_store(std::ostream& o, const U& a);

  void draw_task_store(std::ostream& o) const;

  domain::TasksMirror model_;
  bool miss_;  // кеш устарел
  boost::shared_ptr<pq_dal::PQConnectionPool> pool_;
};
}

#endif // BUSI_H
