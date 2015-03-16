#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "common/app_types.h"

#include <set>
#include <string>
#include <vector>

namespace entities {
// По идее это указание Виду снизу. Это плохо
struct TaskViewTableIdx {
  const static int kId = 0;
  const static int kTaskName = 1;
  const static int kPriority = 2;
  const static int kDone = 3;
};

// FIXME: для таких объектов важно equal and hash!!
// FIXME: закончить реализацию
// Есть одно но. Внутри нет быстрого поиска по id.
//   можно сделать хэш таблицей, и наверное это правильно, т.к.
//   это работает как кеш.
//
// Придает семантику значений
// Ухудшает локальность кеша
// FIXME: Immutable now?
class TaskValue {
public:
  static TaskValue create();
  static TaskValue create(const std::string& d, const int p);
  static TaskValue create(const int id, const std::string& d, const int p);
  static TaskValue create(const int id, const std::string& d, const int p, const bool done);
  TaskValue(const int id, const std::string& d, const int p, const bool);

  // copy/assign
  TaskValue(const TaskValue& v);
  TaskValue& operator=(const TaskValue& v);

  size_t id;
  std::string m_name;  // FIXME: NonImmutable really
  int m_priority;
  bool done;  // need store
};

// FIXME: должны быть уникальные по имени и при создании это нужно контролировать.
class Tag {
public:
  Tag(size_t id, const std::string& name)
    : m_primaryKey(id)
    , m_name(name)
    , m_color("green"){ }

  size_t m_primaryKey;
  std::string m_name;
  std::string m_color;
};

struct EntityStates {
  static const size_t kInactiveKey;
  static const int kDefaultPriority;
  static const bool kNonDone;
};

/**
  раз обрабатываем пачкой, то наверное нужны метки
  updated, to delete, ...
  Feature - пока нет
  Origin est., Current est., Elapsed, Remain

  \fixme нужно как-то сохранять со связями

  http://stackoverflow.com/questions/308276/c-call-constructor-from-constructor
*/
class TaskEntity {
public:
  // builders
  static app::SharedPtr<TaskEntity> create(const std::string& task_name);
  static app::SharedPtr<TaskEntity> create(const entities::TaskValue& v);

  // ctor/dtor/assign/copy
  TaskEntity();

  // conv
  entities::TaskValue toValue() const;
  void assign(const entities::TaskValue& v);

  // data
  size_t id;  // нужно какое-то не активное
  std::string m_name;
  int m_priority;
  bool m_isDone;
};

class TagEntity {

};

// set лучше, но до сохранения индекс может быть не уникальным
typedef app::SharedPtr<TaskEntity> TaskEntityPtr;
typedef app::SharedPtr<const TaskEntity> ImmutableTaskEntityPtr;
typedef std::vector<entities::TaskEntityPtr> Tasks;
}  // namespace..
#endif // DOMAIN_H
