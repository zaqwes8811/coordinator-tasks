#ifndef ISOLATION_H
#define ISOLATION_H

namespace isolation {
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

class ModelListener_virtual {
public:
  virtual ~ModelListener_virtual() { }
  void update() { do_update(); }

private:
  virtual void do_update() = 0;
};

typedef app::SharedPtr<isolation::ModelListener_virtual> ModelListenerPtr;

}  // space

#endif // ISOLATION_H
