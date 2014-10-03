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
// Origin est.
// Current est.
// Elapsed
// Remain
//
// TODO: нужно как-то сохранять со связями
//std::set<std::string> labels_;  // TODO: сделать связь, но пока не нужно
//   weak_ptr<...>  - не должен владеть
//
// действия
//bool update;  // вообще это плохо - это портить консистентность
//bool to_delete;  // удалять ничего не нужно по сути-то 
//bool is_done;  // work was done
//void initialize()  // bad, but
//explicit TaskEntity(std::string& name) :
class TaskEntity {
public:
  // http://stackoverflow.com/questions/308276/c-call-constructor-from-constructor
  TaskEntity() 
    : primary_key_(EntitiesStates::kInActiveKey)
    , priority_(EntitiesStates::kDefaulPriority)
    , is_done_(false) { }

  int get_primary_key() const { return primary_key_; }

  static boost::shared_ptr<TaskEntity> create(const std::string& task_name);

  // лучше по значению
  std::string get_task_name() const { return task_name_; }
  void set_task_name(const std::string& value) { task_name_ = value; }
  
  int get_priority() const { return priority_; }
  void set_priority(const int val) { priority_ = val; }

  bool get_is_done() const
  { return is_done_; }

  // лучше так чем городить условную логику
  void set_is_done(bool val)
  { is_done_ = val; }

private:
  friend class pq_dal::TaskLifetimeQueries;  // только он меняет первичный ключ

  void set_primary_key_(int val) { primary_key_ = val; }

  int primary_key_;  // нужно какое-то не активное
  std::string task_name_;
  int priority_;
  bool is_done_;
};

// set лучше, но до сохранения индекс может быть не уникальным
typedef std::vector<boost::shared_ptr<entities::TaskEntity> > Tasks;

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
}  // namespace..

namespace values {
// FIXME: закончить реализацию
// Есть одно но. Внутри нет быстрого поиска по id.
//   можно сделать хэш таблицей, и наверное это правильно, т.к.
//   это работает как кеш.
class TaskValue {
public:
    static TaskValue create(const std::string& d, const int p) {
      return TaskValue(entities::EntitiesStates::kInActiveKey, d, p);
    }

    static TaskValue create() {
      int p = entities::EntitiesStates::kDefaulPriority;
      return TaskValue(entities::EntitiesStates::kInActiveKey, std::string(), p);
    }

    const int id;
    const int priority;
    //const bool done;  // need store

    // Придает семантику значений
    // Ухудшает локальность кеша
    boost::shared_ptr<const std::string> description;

private:
    TaskValue(const int _id, const std::string& d, const int p)
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

#endif // DOMAIN_H
