#ifndef DOMAIN_H
#define DOMAIN_H

// FIXME: BAD!! верхний уровень знает о нижнем
// возможно можно сделать класс на соседнем уровне?
#include "canary/storage_access.h"  

//#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <set>
#include <string>

namespace domain {
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
    , priority_(EntitiesStates::kDefaulPriority) { }

  int get_primary_key() const { return primary_key_; }

  static boost::shared_ptr<TaskEntity> create(std::string& task_name) {
    boost::shared_ptr<TaskEntity> tmp = boost::make_shared<TaskEntity>(TaskEntity());
    tmp->task_name_ = task_name;
    return tmp;
  }

  // лучше по значению
  std::string get_task_name() const { return task_name_; }
  void set_task_name(std::string& value) { task_name_ = value; }
  
  int get_priority() const { return priority_; }

private:
  friend class dal::TaskLifetimeQueries;  // только он меняет первичный ключ

  void set_primary_key_(int val) { primary_key_ = val; }
  int primary_key_;  // нужно какое-то не активное
  
  // Task name
  std::string task_name_;

  // Priority
  int priority_;
};

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

#endif // DOMAIN_H
