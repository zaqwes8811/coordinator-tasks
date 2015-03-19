#include "heart/config.h"

#include <data_access_layer/sqlite_queries.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <iostream>

using std::string;

namespace new_space {

template <typename T> void registerBeanClass(T& obj) { obj.self_->registerBeanClass_(); }
template <typename T> void drop(T& obj) { obj.self_->drop_(); }

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

    void registerBeanClass_() override { data_.registerBeanClass();  }
    void drop_() override { data_.drop(); }

    T data_;
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

  1. Put handler - db specific - to queries builder
  2. Builder store in some high level object
  3. Want make queries on base getted handler

  ORM thinks:
    https://msdn.microsoft.com/en-us/magazine/dd569757.aspx
*/
class db_manager_concept_t {
public:
  template<typename T>
  db_manager_concept_t(const T& x) : self_(std::make_shared<model<T>>(std::move(x)))
  { }

  new_space::table_concept_t getTaskTableQuery()
  {  return self_->getTaskTableQuery_(); }

  new_space::table_concept_t getTaskTagQuery()
  {  return self_->getTaskTagQuery_(); }

private:
  class concept_t {
  public:
    virtual ~concept_t() = default;
    virtual new_space::table_concept_t getTaskTableQuery_() = 0;
    virtual new_space::table_concept_t getTaskTagQuery_() = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(std::move(x)) { }

    new_space::table_concept_t getTaskTableQuery_() override
    { return data_.getTaskTableQuery(); }

    new_space::table_concept_t getTaskTagQuery_() override
    { return data_.getTaskTagQuery(); }

    T data_;
  };

  std::shared_ptr<
    //const  // can't
    concept_t> self_;
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

  sqlite_queries::SQLiteTagTableQuery  getTaskTagQuery() {
    return sqlite_queries::SQLiteTagTableQuery(m_connPtr);
  }

private:
  // FIXME: important not only lifetime, but connection state to!
  gc::SharedPtr<sqlite3_cc::sqlite3> m_connPtr;
  const std::string m_taskTableName;
};

// by value, not by type
enum db_vars { DB_SQLITE, DB_POSTGRES };

db_manager_concept_t build_database(const int selector) {
  if (selector == DB_SQLITE) {
    return db_manager_concept_t(database_app::SQLiteDataBase());
  } else {
    return db_manager_concept_t(database_app::SQLiteDataBase());
  }
}
}

TEST(ConceptsTest, Test) {
  using namespace new_space;

  auto db = database_app::build_database(database_app::DB_SQLITE);
  auto tables = std::vector<table_concept_t>{db.getTaskTableQuery(), db.getTaskTagQuery()};

  for (auto& a : tables)
    registerBeanClass(a);

  for (auto& a : tables)
    drop(a);
}

TEST(ConceptsTest, ActorEnv) {
  // FIXME: .then() how help if future is out of scope - see Sutter
  // FIXME: may be shared future?
  // FIXME: "push" and "pull" models
  // FIXME: need in another context, not in it actor - Actors and Wrappers!
  // Actor - Wrapper - Wrapper - Actor

  // FIXME: actor doubled interface!
  using namespace new_space;

  // trouble with handles in actor model
  // need real links
  // Object own it, but run in other thread
  auto db = database_app::build_database(database_app::DB_SQLITE);
  auto tables = std::vector<table_concept_t>{db.getTaskTableQuery(), db.getTaskTagQuery()};

  auto action = [tables] {
    for (auto& a : tables)
      registerBeanClass(a);
  };

  action();

  for (auto& a : tables)
    drop(a);
}
