/**
  http://stackoverflow.com/questions/21821948/concept-based-polymorphism
  http://thinkthencode.blogspot.ru/2013/02/concept-based-polymorphism.html

*/

#include <typeinfo>

#include <gtest/gtest.h>

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

using std::ostream;
using std::string;
using std::endl;
using std::cout;
using std::vector;
using std::move;

namespace ps_sample_shared_extend {
class object_t {
public:
  template<typename T>
  object_t(const T& x) : self_(std::make_shared<model<T>>(move(x)))
  { }

  friend void draw(const object_t &x, ostream &out, size_t position)
  {
    x.self_->draw_(out, position);
    //cout << x
  }

private:
  class concept_t {
  public:
    virtual ~concept_t() = default;
    virtual void draw_(ostream& out, size_t position) const = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(move(x)) { }
    void draw_(ostream& out, size_t position) const {
      //draw(data_, out, position);
    }

    T data_;  // главный вопрос в куче ли? Да - см в Мейсере 35
  };

  std::shared_ptr<const concept_t> self_;  // ссылки на immutable
};

using document_t = vector<object_t>;  // полиморфизм только через shared_ptrs

template<typename T>
void draw(const T& x, ostream& out, size_t position)
{ out << string(position, ' ') << x << endl; }


void draw(const document_t&x, ostream &out, size_t position)
{
  out << string(position, ' ') << "<document>" << endl;
  for (const auto& e : x) draw(e, out, position + 2);
  out << string(position, ' ') << "</document>" << endl;
}

/// Ps
using history_t = vector<document_t>;
void commit(history_t& x) {
  assert(x.size());
  x.push_back(x.back());
}

void undo(history_t& x) { assert(x.size()); x.pop_back(); }

document_t& current(history_t& x) {
  assert(x.size());
  return x.back();
}
}

namespace external_space  {
// не нужно ничего наследовать.
class my_class_t {
public:
  my_class_t() {}
  my_class_t(const my_class_t&) { cout << "copy mc\n"; }
  int arr[10];

private:
  // FIXME: What the hell?
  static void *operator new (size_t size);
  static void operator delete(void *ptr);
};

void draw(const my_class_t&, ostream& out, size_t position)
{ out << string(position, ' ') << "my_class_t()" << endl; }
}

TEST(EvelPsExtend, App) {
  using namespace ps_sample_shared_extend;
  using namespace external_space;
  history_t h(1);

  // Работаем с верхним элементом
  //current(h).emplace_back(0);
  //current(h).emplace_back(string("Hello!"));
  current(h).emplace_back(my_class_t());
  current(h).emplace_back(my_class_t());
  current(h).emplace_back(my_class_t());

  draw(current(h), cout, 0);
  cout << "-------------" << endl;
  std::reverse(current(h).begin(), current(h).end());

  commit(h);  // сохраняем текущую и ее копируем на верх.
  cout << "-------------" << endl;

  //current(h).emplace_back(current(h));
  current(h).emplace_back(my_class_t());
  current(h)[4] = my_class_t();

  draw(current(h), cout, 0);
  cout << "-------------" << endl;

  undo(h);
  draw(current(h), cout, 0);
}

namespace database {
/**
  \fixme const troubles

  \attention how reduce 2 hier.?
*/
namespace detail {
struct pq_tag { };
struct sqlite_tab { };

template <typename T> struct holder_traits;
template <> struct holder_traits<string> {
  typedef sqlite_tab category;
};

template <> struct holder_traits<int> {
  typedef pq_tag category;
};
}

namespace real_objs {
// no inh.!
struct sqlite {
  sqlite(string _s) : s(_s) { }
  void drop() { }
  string build() {
    return s;
  }

  string s;
};

struct sqlite_builder {
  //sqlite_builder()
};

struct postgresql {
  postgresql(int _s) : s(_s) { }
  void drop() { }

  int build() {
    return s;
  }

  int s;
};
}  // space

// Dropable
class object_t {
public:
  template<typename T>
  object_t(const T& x) : self_(std::make_shared<model<T>>(move(x)))
  { }

  // FIXME: it's bad. must be friend?
  void drop()
  {
    // no way to know dyn. type
    self_->drop_();
  }

  // generate
  //template <typename T

private:
  class concept_t {
  public:
    virtual ~concept_t() = default;
    virtual void drop_() = 0;
    virtual ps_sample_shared_extend::object_t build_() = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(move(x)) { }
    void drop_() override
    { data_.drop(); }

    ps_sample_shared_extend::object_t build_() override {
      return ps_sample_shared_extend::object_t(data_.build());
    }

    T data_;
  };

  std::shared_ptr<
    //const  // can't
    concept_t> self_;  // ссылки на immutable
};

// Fabric:
template<typename T>
object_t create(std::weak_ptr<T> p) {
  return object_t(0);
}

// by value, not by type
enum db_vars { DB_SQLITE, DB_POSTGRES };

//if (selector == DB_POSTGRES)
object_t build_data_base(const int selector) {
  using namespace real_objs;
  if (selector == DB_SQLITE)
    return object_t(sqlite(""));
  else
    return object_t(postgresql(0));
}
}

TEST(DB, Test) {
  using namespace database;
  using namespace database::real_objs;

  // db.registerBeanClass<Obj>()
  auto a = object_t(sqlite(""));
  auto b = object_t(postgresql(0));
  a.drop();

  // FIXME: how connect multy DB drivers?

  int selector = 0;
  auto db = build_data_base(selector);

  // 1. Put handler - db specific - to queries builder
  // 2. Builder store in some high level object
  // 3. Want make queries on base getted handler
}
