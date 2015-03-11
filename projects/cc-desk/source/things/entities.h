#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "common/app_types.h"

#include <set>
#include <string>
#include <vector>

namespace values {
class ImmutableTask;
}

namespace entities {
const std::string gTableName = "tasks";

struct EntitiesStates {
  static const int kInActiveKey;
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

  static app::SharedPtr<TaskEntity> create(const values::ImmutableTask& v);

  values::ImmutableTask make_value() const;

  void assign(const values::ImmutableTask& v);

  // accessors
  int get_primary_key() const;

  void set_primary_key(int val);

  std::string get_task_name() const;

  void set_task_name(const std::string& value);

  int get_priority() const;

  void setPriority(const int val);

  bool getIsDone() const;

  void set_is_done(bool val);

private:
  //friend class pq_dal::TaskLifetimeQueries;  // только он меняет первичный ключ
  int id_;  // нужно какое-то не активное
  std::string task_name_;
  int priority_;
  bool is_done_;
};

// set лучше, но до сохранения индекс может быть не уникальным
typedef app::SharedPtr<TaskEntity> TaskEntityPtr;
typedef app::SharedPtr<const TaskEntity> ImmutableTaskEntityPtr;
typedef std::vector<entities::TaskEntityPtr> Tasks;
}  // namespace..
#endif // DOMAIN_H
