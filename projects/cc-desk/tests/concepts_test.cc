#include "heart/config.h"

#include <data_access_layer/sqlite_queries.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <iostream>

using std::string;

namespace new_space {
class object_t {
public:
  template<typename T>
  object_t(const T& x) : self_(std::make_shared<model<T>>(std::move(x)))
  { }

  void registerBeanClass()
  {
    self_->registerBeanClass_();
  }

private:
  class concept_t {
  public:
    virtual ~concept_t() = default;
    virtual void registerBeanClass_() const = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(std::move(x)) { }
    void registerBeanClass_() const {
      data_.registerBeanClass();
    }

    T data_;  // главный вопрос в куче ли? Да - см в Мейсере 35
  };

  std::shared_ptr<const concept_t> self_;  // ссылки на immutable
};
}  // space

namespace database_app {
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

// Dropable
class object_t {
public:
  template<typename T>
  object_t(const T& x) : self_(std::make_shared<model<T>>(std::move(x)))
  { }

  // FIXME: it's bad. must be friend?
  void drop()
  {
    // no way to know dyn. type
    self_->drop_();
  }

private:
  class concept_t {
  public:
    virtual ~concept_t() = default;
    virtual void drop_() = 0;
    //virtual new_space::object_t build_() = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(std::move(x)) { }
    void drop_() override
    { data_.drop(); }

    //new_space::object_t build_() override {
    //  return new_space::object_t(data_.build());
    //}

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
  //using namespace real_objs;
  //if (selector == DB_SQLITE)
  //  return object_t(sqlite(""));
  //else
  //  return object_t(postgresql(0));
}
}

TEST(ConceptsTest, Test) {
  using namespace database_app;

  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");
  auto tasks = sqlite_queries::SQLiteTaskTableQueries(h, models::kTaskTableNameRef);
  auto obj = object_t(tasks);
  obj.drop();

  // db.registerBeanClass<Obj>()
  //auto a = object_t(sqlite(""));
  //auto b = object_t(postgresql(0));
  //a.drop();

  // FIXME: how connect multy DB drivers?

  //int selector = 0;
  //auto db = build_data_base(selector);

  // 1. Put handler - db specific - to queries builder
  // 2. Builder store in some high level object
  // 3. Want make queries on base getted handler
}
