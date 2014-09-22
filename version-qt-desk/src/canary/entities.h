#ifndef DOMAIN_H
#define DOMAIN_H

// FIXME: BAD!! верхний уровень знает о нижнем
// возможно можно сделать класс на соседнем уровне?
#include "canary/storage_access.h"  

//#include <boost/noncopyable.hpp>

#include <set>
#include <string>

namespace domain {
const std::string gTableName = "tasks";

struct entities_states {
  static const int kInActiveKey = -1;
  static const int kDefaulPriority = 0;
};

// раз обрабатываем пачкой, то наверное нужны метки
// updated, to delete, ...
class Task {
public:
  Task() 
    : primary_key_(entities_states::kInActiveKey)
    , priority_(entities_states::kDefaulPriority) { }

  int get_primary_key() const { return primary_key_; }

  // лучше по значению
  std::string get_task_name() const { return task_name_; }
  
  int get_priority() const { return priority_; }

private:
  friend class dal::TaskLifetimeQueries;  // только он меняет первичный ключ
  void set_primary_key_(int val) { primary_key_ = val; }
  // Уникальный для каждой задачи
  int primary_key_;  // нужно какое-то не активное

  // Джоел первая.
  // Feature - пока нет
  // Task name
  std::string task_name_;

  // Priority
  int priority_;

  // Origin est.
  // Current est.
  // Elapsed
  // Remain

  // TODO: нужно как-то сохранять со связями
  //std::set<std::string> tags_;  // TODO: сделать связь, но пока не нужно

  // действия
  //bool update;  // вообще это плохо - это портить консистентность
  //bool to_delete;  // удалять ничего не нужно по сути-то 
  //bool is_done;  // work was done
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
