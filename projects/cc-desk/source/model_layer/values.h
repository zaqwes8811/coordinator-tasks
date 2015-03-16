#ifndef VALUES_H
#define VALUES_H

#include "common/app_types.h"

#include <string>

namespace values {
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
class Task {
public:
  static Task create();
  static Task create(const std::string& d, const int p);
  static Task create(const int id, const std::string& d, const int p);
  static Task create(const int id, const std::string& d, const int p, const bool _done);

  // copy/assign
  Task(const Task& v);
  Task& operator=(const Task& v);

  // accessors
  size_t id() const;
  app::SharedPtr<const std::string> description() const;
  int priority() const;
  bool done() const;

private:
  int _id;
  app::SharedPtr<const std::string> _description;  // FIXME: NonImmutable really
  int _priority;
  bool _done;  // need store

  Task(const int _id, const std::string& d, const int p, const bool);
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
}


#endif // VALUES_H
