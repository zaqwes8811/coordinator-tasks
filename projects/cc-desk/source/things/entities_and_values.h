#ifndef DOMAIN_H_
#define DOMAIN_H_

#include "top/app_types.h"

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

  void set_priority(const int val);

  bool get_is_done() const;

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

namespace values {
// FIXME: для таких объектов важно equal and hash!!
// FIXME: закончить реализацию
// Есть одно но. Внутри нет быстрого поиска по id.
//   можно сделать хэш таблицей, и наверное это правильно, т.к.
//   это работает как кеш.
//
// Придает семантику значений
// Ухудшает локальность кеша
// FIXME: Immutable now?
class ImmutableTask {
public:
  static ImmutableTask create();
  static ImmutableTask create(const std::string& d, const int p);
  static ImmutableTask create(const int id, const std::string& d, const int p);
  static ImmutableTask create(const int id, const std::string& d, const int p, const bool _done);

  // copy/assign
  ImmutableTask(const ImmutableTask& v);
  ImmutableTask& operator=(const ImmutableTask& v);

  // accessors
  int id() const;
  app::SharedPtr<const std::string> description() const;
  int priority() const;
  bool done() const;

private:
  int _id;
  app::SharedPtr<const std::string> _description;  // FIXME: NonImmutable really
  int _priority;
  bool _done;  // need store

  ImmutableTask(const int _id, const std::string& d, const int p, const bool);
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
  Tag(const std::string& name) : m_primary_key(), m_name(name) { }

//private:
  size_t m_primary_key;
  std::string m_name;
  std::string m_color;
};
}

#endif // DOMAIN_H
