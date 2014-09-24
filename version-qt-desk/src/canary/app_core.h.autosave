#ifndef BUSI_H
#define BUSI_H

#include "canary/entities.h"
#include "canary/pq_dal.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace app_core
{
class AppCore
   : boost::noncopyable {
public:
    AppCore(
            domain::Model _model,
            boost::shared_ptr<pq_dal::PQConnectionPool> _pool)
        : clear(false), miss_(true), pool_(_pool) {
        model_ = (_model);  // assign
        miss_ = false;  // последние данные загружены
    }

  // наверное лучше сразу сохранить
  // добавлять все равно буду скорее всего по-одному
  void append(domain::Model::value_type e);

  //
  void save();

  static AppCore* heapCreate(
      boost::shared_ptr<pq_dal::PQConnectionPool>);

  bool clear;  // удаляем ли созданное, нужно для тестирования

  ~AppCore();

  // persist filters:
  void load_all();
  void load_active();

  // render filters:

private:
  domain::Model model_;
  bool miss_;  // кеш устарел
  boost::shared_ptr<pq_dal::PQConnectionPool> pool_;
};
}

#endif // BUSI_H
