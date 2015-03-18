#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "common/app_types.h"

#include <set>
#include <string>
#include <vector>

namespace entities {
// FIXME: По идее это указание Виду снизу. Это плохо
struct TaskViewTableIdx {
  const static int kId = 0;
  const static int kTaskName = 1;
  const static int kPriority = 2;
  const static int kDone = 3;
};

struct EntityStates {
  static const size_t kInactiveKey;
  static const int kDefaultPriority;
  static const bool kDefaultDone;
};

class Task;
typedef gc::SharedPtr<Task> TaskEntity;

/**
  FIXME: для таких объектов важно equal and hash!!
  FIXME: закончить реализацию
  Есть одно но. Внутри нет быстрого поиска по id.
   можно сделать хэш таблицей, и наверное это правильно, т.к.
   это работает как кеш.

  Придает семантику значений
  Ухудшает локальность кеша
  FIXME: Immutable now?

  раз обрабатываем пачкой, то наверное нужны метки
  updated, to delete, ...
  Feature - пока нет
  Origin est., Current est., Elapsed, Remain

  \fixme нужно как-то сохранять со связями

  http://stackoverflow.com/questions/308276/c-call-constructor-from-constructor
*/
class Task {
public:
  // builders
  static TaskEntity createEntity(const entities::Task& v);

  // conv
  entities::Task toValue() const;

  // ctor/dtor/assign/copy
  Task(const Task& v);
  Task();

  // used overload
  //void id(size_t id);
  //size_t id() const;

  // data
  size_t id;  // нужно какое-то не активное
  std::string name;
  int priority;
  bool done;
};

class Tag;
typedef gc::SharedPtr<entities::Tag> TagEntity;

/**
  \fixme должны быть уникальные по имени и при создании это нужно контролировать.
*/
class Tag {
public:
  Tag() : id(EntityStates::kInactiveKey) { }
  Tag(size_t _id, const std::string& name)
    : id(_id)
    , name(name)
    , color("green") { }

  Tag toValue() const;

  size_t id;
  std::string name;
  std::string color;
};

typedef std::vector<entities::TaskEntity> TaskEntities;
}  // namespace..
#endif // DOMAIN_H
