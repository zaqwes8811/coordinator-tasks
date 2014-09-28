#ifndef ISOLATION_H
#define ISOLATION_H

namespace isolation {
// цель - не включать заголовоки видов в модель - нужна виртуальность
// попробую статический полиморфизм.
template <typename View>  // тогда и модель становится шаблоном, а значит код выносится в заголовки
class ModelListenerStaticPolym {
public:
  // Не владеет
  ModelListenerStaticPolym(View* const view) : view_(view) { }

  void update() {
    //view_->
  }

private:
  View* const view_;
};

class ModelListenerMediatorDynPolym {
public:
  virtual ~ModelListenerMediatorDynPolym() { }
  void update() {
    //
    update_();
  }
private:
  virtual void update_() = 0;
};
}

#endif // ISOLATION_H
