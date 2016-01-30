#ifndef CORE_CONCEPTS_H_
#define CORE_CONCEPTS_H_

#include "entities.h"

#include <memory>
#include <string>

namespace concepts {

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

/**
  \attention Small life time!

  Делать один репозиторий не советуют

  FIXME: Может DI какой сделать, или все равно?

  FIXME: как то объединить create, update, etc. в persist

  Назначет id!! очень важно! объекты уникальные
  Создает, если еще не был создан, либо обновляет всю запись
  by value
  На групповую вставку могут быть ограничения, но в данной задаче
   пока не нужно, если не нужно будет что-то куда-то автоматически переливать.

  FIXME: с умными указателями возникают проблемы с константростью!
*/
class lifetime_concept_t {
public:
  template<typename T>
  lifetime_concept_t(const T& x) : self_(std::make_shared<model<T>>(std::move(x)))
  { }

  entities::Task persist(const entities::Task& v)
  { return self_->persist_(v); }

  void update(const entities::Task& v)
  { self_->update_(v); }

  entities::TaskEntities loadAll() const
  { return self_->loadAll_(); }

private:

  class concept_t {
  public:
    virtual ~concept_t() = default;
    virtual entities::Task persist_(const entities::Task& v) = 0;
    virtual void update_(const entities::Task& v) = 0;
    virtual entities::TaskEntities loadAll_() const = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(std::move(x)) { }

    virtual entities::Task persist_(const entities::Task& v) OVERRIDE
    { return data_.persist(v); }

    void update_(const entities::Task& v) OVERRIDE
    { data_.update(v); }

    entities::TaskEntities loadAll_() const OVERRIDE
    { return data_.loadAll(); }

    T data_;
  };

  std::shared_ptr<
    //const
    concept_t> self_;  // ссылки на immutable
};

/**
  \fixme const troubles

  \attention how reduce 2 hier.?
*/
namespace detail {
struct pq_tag { };
struct sqlite_tab { };

template <typename T> struct holder_traits;
template <> struct holder_traits<std::string> {
  typedef sqlite_tab category;
};

template <> struct holder_traits<int> {
  typedef pq_tag category;
};
}

/**
  \attention Only in single thread! Actors Model can help

  http://herbsutter.com/2013/05/30/gotw-90-solution-factories/

  \fixme How make noncopyble in C++11

  \fixme: strange design. May be bad lifetimes

  http://www.drdobbs.com/cpp/c11-uniqueptr/240002708

  \brief Dropable

  \fixme Tourble. We return query objects with internal handler

  \attention Query objects lifetime << manager lifetime

  1. Put handler - db specific - to queries builder
  2. Builder store in some high level object
  3. Want make queries on base getted handler

  ORM thinks:
    https://msdn.microsoft.com/en-us/magazine/dd569757.aspx

  \attention !!! How to know what thread delete it? Especialy if it's shared ptr!
    Hm... But not races if std::shared_ptr, destruction under lock?
*/
class db_manager_concept_t {
public:
  template<typename T>
  db_manager_concept_t(const T& x) : self_(std::make_shared<model<T>>(std::move(x)))
  { }

  //template <typename Q>
  //concepts::table_concept_t table();

  concepts::table_concept_t getTaskTableQuery()
  {  return self_->getTaskTableQuery_(); }

  concepts::lifetime_concept_t getTaskLifetimeQuery()
  {  return self_->getTaskLifetimeQuery_(); }

  concepts::table_concept_t getTagTableQuery()
  {  return self_->getTagTableQuery_(); }

private:
  class concept_t {
  public:
    virtual ~concept_t() = default;
    virtual concepts::table_concept_t getTaskTableQuery_() = 0;
    virtual concepts::table_concept_t getTagTableQuery_() = 0;
    virtual concepts::lifetime_concept_t getTaskLifetimeQuery_() = 0;
  };

  template<typename T>
  struct model : concept_t {
    model(const T& x) : data_(std::move(x)) { }

    concepts::table_concept_t getTaskTableQuery_() OVERRIDE
    { return data_.getTaskTableQuery(); }

    concepts::table_concept_t getTagTableQuery_() OVERRIDE
    { return data_.getTagTableQuery(); }

    concepts::lifetime_concept_t getTaskLifetimeQuery_() OVERRIDE
    { return data_.getTaskLifetimeQuery(); }

    T data_;
  };

  std::shared_ptr<
    //const  // can't
    concept_t> self_;
};
}
#endif
