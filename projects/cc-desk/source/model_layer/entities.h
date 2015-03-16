#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "common/app_types.h"
#include "values.h"

#include <set>
#include <string>
#include <vector>

namespace entities {
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
  static app::SharedPtr<TaskEntity> create(const values::Task& v);

  // ctor/dtor/assign/copy
  TaskEntity();

  values::Task toValue() const;

  void assign(const values::Task& v);

  // FIXME: remove it
  size_t getId() const
  { return m_primaryKey; }

  std::string get_task_name() const
  { return m_name; }

  bool getIsDone() const
  { return m_isDone; }

  int get_priority() const
  { return m_priority; }

  size_t m_primaryKey;  // нужно какое-то не активное
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
