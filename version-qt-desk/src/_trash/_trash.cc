#include "top/config.h"

#include "canary/entities.h"

#include <boost/shared_ptr.hpp>

namespace dirty {
using namespace entities;

// TROUBLES: при работе с shared_ptrs
template <class OP1, class OP2
  //, typename R
>
class compose_f_gx_t
  //: std::unary_function<typename R /*OP2/*::argument_type*/, typename OP1::result_type>
  {
private:
  OP1 op1;
  OP2 op2;

public:
  compose_f_gx_t(const OP1& o1, const OP2& o2) : op1(o1), op2(o2) { }

  template <typename R>
  typename OP1::result_type
  operator()(
    //const typename OP2::argument_type
    R& x) const {  //
    typename OP1::result_type tmp = op1(op2(x));
    return tmp;
  }
};

template <class OP1, class OP2>
inline compose_f_gx_t<OP1, OP2>
compose_f_gx(const OP1& o1, const OP2& o2) {
  return compose_f_gx_t<OP1, OP2>(o1, o2);
}

template <class OP1>
class store_t
  : std::unary_function<typename OP1::argument_type, typename OP1::result_type>
  {
private:
  OP1 op1;

public:
  explicit store_t(const OP1& o1) : op1(o1) { }

  typename OP1::result_type operator()(const typename OP1::argument_type& x) const {  //
    typename OP1::result_type tmp = op1(x);
    return tmp;
  }
};

template <class OP1>
inline store_t<OP1>
store(const OP1& o1) {
  return store_t<OP1>(o1);
}

// создается лишний класс
struct is_saved {
  bool operator()(boost::shared_ptr<TaskEntity> e) {
    return true;
  }
};

template <typename Auto>
struct auto_cont {
  Auto a;
  explicit auto_cont(Auto& _a) : a(_a) {}
  Auto get() const { return a; }
};

template <typename A>
auto_cont<A> inline get_a(A& a) {
  return auto_cont<A>(a);
}
}

namespace stolen {
// http://rsdn.ru/forum/cpp/342382.all
template<typename T>
struct smart2ptr_t
{
    //typedef X argument_type;  // FIXME: как сделать то?

    T f;

    smart2ptr_t(T t) : f(t) {}

    template<typename R>
    void operator()(R &p)
    {
        f(p.get());
    }
};

template<typename T>
smart2ptr_t<T> smart2ptr(T op)
{
    return smart2ptr<T>(op);
}
}
