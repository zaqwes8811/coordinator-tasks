#ifndef DOMAIN_H_
#define DOMAIN_H_

//#include "canary/pq_dal.h"

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <set>
#include <string>
#include <vector>

namespace pq_dal {
    class TaskLifetimeQueries;  // lower level?
}

namespace entities {
const std::string gTableName = "tasks";

struct EntitiesStates {
  static const int kInActiveKey = -1;
  static const int kDefaulPriority = 0;
};

// раз обрабатываем пачкой, то наверное нужны метки
// updated, to delete, ...
// Feature - пока нет
// Origin est., Current est., Elapsed, Remain
//
// TODO: нужно как-то сохранять со связями
//
class TaskEntity {
public:
  // ctor/dtor/assign/copy
  // http://stackoverflow.com/questions/308276/c-call-constructor-from-constructor
  // FIXME: конструктор лучше закрыть
  TaskEntity();
  static boost::shared_ptr<TaskEntity> create(const std::string& task_name);

  int get_primary_key() const;

  std::string get_task_name() const;
  void set_task_name(const std::string& value);

  int get_priority() const;
  void set_priority(const int val);

  bool get_is_done() const;
  void set_is_done(bool val);

private:
  friend class pq_dal::TaskLifetimeQueries;  // только он меняет первичный ключ

  // FIXME: а может в конструктор передать и все?
  void set_primary_key_(int val)
  { primary_key_ = val; }

  int primary_key_;  // нужно какое-то не активное
  std::string task_name_;
  int priority_;
  bool is_done_;
};

// set лучше, но до сохранения индекс может быть не уникальным
typedef boost::shared_ptr<TaskEntity> TaskEntityPtr;
typedef std::vector<entities::TaskEntityPtr> Tasks;
// FIXME: не удобно
//typedef Tasks::value_type Tasks::elem_ptr;
//typedef Tasks::value_type::element_type elem;
}  // namespace..

namespace values {
// FIXME: для таких объектов важно equal and hash!!
// FIXME: закончить реализацию
// Есть одно но. Внутри нет быстрого поиска по id.
//   можно сделать хэш таблицей, и наверное это правильно, т.к.
//   это работает как кеш.
class ImmutableTask {
public:
    static ImmutableTask create(const std::string& d, const int p) {
      return ImmutableTask(entities::EntitiesStates::kInActiveKey, d, p);
    }

    static ImmutableTask create() {
      int p = entities::EntitiesStates::kDefaulPriority;
      return ImmutableTask(entities::EntitiesStates::kInActiveKey, std::string(), p);
    }

    const int id;
    const int priority;
    //const bool done;  // need store

    // Придает семантику значений
    // Ухудшает локальность кеша
    const  // FIXME: Immutable now?
    boost::shared_ptr<const std::string> description;  // FIXME: NonImmutable really

private:
    ImmutableTask(const int _id, const std::string& d, const int p)
        : id(_id)
        , priority(p)
        , description(new std::string(d)) { }
};

// По идее это указание Виду снизу. Это плохо
struct TaskViewTableIdx {
  const static int kId = 0;
  const static int kTaskName = 1;
  const static int kPriority = 2;
  const static int kDone = 3;
};
}

namespace canary {
//std::set<std::string> labels_;  // TODO: сделать связь, но пока не нужно
//   weak_ptr<...>  - не должен владеть

// TODO: должны быть уникальные по имени и при создании это нужно контролировать.
// TODO: Как быть при обновлении имени или цвета? Возможно нужно хранить shared_ptrs не на константу.
class Tag {
public:
  Tag(const std::string& name) : primary_key_(), name_(name) { }

private:
  int primary_key_;
  std::string name_;
  std::string color_;
};
}

#endif // DOMAIN_H
