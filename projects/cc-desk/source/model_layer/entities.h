#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "common/app_types.h"
#include "values.h"

#include <set>
#include <string>
#include <vector>

namespace entities {
struct EntityStates {
  static const size_t kInActiveKey;
  static const int kDefaultPriority;
  static const bool kNonDone;
};

// раз обрабатываем пачкой, то наверное нужны метки
// updated, to delete, ...
// Feature - пока нет
// Origin est., Current est., Elapsed, Remain
//
// TODO: нужно как-то сохранять со связями
//
// http://stackoverflow.com/questions/308276/c-call-constructor-from-constructor
class TaskEntity {
public:
  // ctor/dtor/assign/copy
  // FIXME: конструктор лучше закрыть
  TaskEntity();

  static app::SharedPtr<TaskEntity> create(const std::string& task_name);

  static app::SharedPtr<TaskEntity> create(const values::Task& v);

  values::Task toValue() const;

  void assign(const values::Task& v);

  // accessors
  int getPrimaryKey() const;

  void setPrimaryKey(int val);

  std::string get_task_name() const;

  void set_task_name(const std::string& value);

  int get_priority() const;

  void setPriority(const int val);

  bool getIsDone() const;

  void set_is_done(bool val);

private:
  size_t m_id;  // нужно какое-то не активное
  std::string task_name_;
  int priority_;
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
