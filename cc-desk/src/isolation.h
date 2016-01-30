#ifndef ISOLATION_H
#define ISOLATION_H

#include "entities.h"

namespace isolation {
//typedef

namespace detail {
/**
  \brief цель - не включать заголовоки видов в модель - нужна виртуальность
  попробую статический полиморфизм.

  \attention тогда и модель становится шаблоном, а значит код выносится в заголовки
*/
template <typename V>
class ModelListener_templ {
public:
  ModelListener_templ(V* const view)
    : m_viewRawPtr(view) { }

  void update() { }

private:
  V* const m_viewRawPtr;
};
}  // space

class ModelListener {
public:
  virtual ~ModelListener() { }
  void update(entities::TaskEntities m) { do_update(m); }

  virtual void DrawErrorMessage(const std::string& message) = 0;

private:
  virtual void do_update(entities::TaskEntities m) = 0;
};

typedef gc::SharedPtr<isolation::ModelListener> ModelListenerPtr;

}  // space

#endif // ISOLATION_H
