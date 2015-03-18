#include "heart/config.h"

#include <data_access_layer/sqlite_queries.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <iostream>

using std::string;

namespace new_space {

template <typename T> void registerBeanClass(T& obj) {
  obj.self_->registerBeanClass_();
}

template <typename T> void drop(T& obj) {
  obj.self_->drop_();
}

class table_concept_t {
public:
  template<typename T>
  table_concept_t(const T& x) : self_(std::make_shared<model<T>>(std::move(x)))
  { }

private:
  template <typename T> friend void registerBeanClass(T& obj);
  template <typename T> friend void drop(T& obj);

  class concept_t {
  public:
    virtual ~concept_t() = default;
    virtual void registerBeanClass_() = 0;
    virtual void drop_() = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(std::move(x)) { }

    void registerBeanClass_() override {
      data_.registerBeanClass();
    }

    void drop_() override {
      data_.drop();
    }

    T data_;  // главный вопрос в куче ли? Да - см в Мейсере 35
  };

  std::shared_ptr<
    //const
    concept_t> self_;  // ссылки на immutable
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

/**
  \brief Dropable

  \fixme Tourble. We return query objects with internal handler

  \attention Query objects lifetime << manager lifetime
*/
class db_manager_concept_t {
public:
  template<typename T>
  db_manager_concept_t(const T& x) : self_(std::make_shared<model<T>>(std::move(x)))
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

    T data_;
  };

  std::shared_ptr<
    //const  // can't
    concept_t> self_;  // ссылки на immutable
};

class SQLiteDataBase {
public:
  SQLiteDataBase()
    : m_connPtr(std::make_shared<sqlite3_cc::sqlite3>("test.db"))
      , m_taskTableName(models::kTaskTableNameRef)
  { }

  sqlite_queries::SQLiteTaskTableQueries  getTaskTableQuery() {
    return sqlite_queries::SQLiteTaskTableQueries(m_connPtr, m_taskTableName);
  }

private:
  // FIXME: important not only lifetime, but connection state to!
  gc::SharedPtr<sqlite3_cc::sqlite3> m_connPtr;
  const std::string m_taskTableName;
};

// Fabric:
template<typename T>
db_manager_concept_t create(std::weak_ptr<T> p) {
  return db_manager_concept_t(0);
}

// by value, not by type
enum db_vars { DB_SQLITE, DB_POSTGRES };

//if (selector == DB_POSTGRES)
db_manager_concept_t build_data_base(const int selector) {
  //using namespace real_objs;
  //if (selector == DB_SQLITE)
  //  return object_t(sqlite(""));
  //else
  //  return object_t(postgresql(0));
}
}

TEST(ConceptsTest, Test) {
  using namespace new_space;

  auto rawHandler = std::make_shared<sqlite3_cc::sqlite3>("test.db");
  auto query = sqlite_queries::SQLiteTaskTableQueries(rawHandler, models::kTaskTableNameRef);
  auto obj = table_concept_t(query);
  registerBeanClass(obj);

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

  drop(obj);
}
