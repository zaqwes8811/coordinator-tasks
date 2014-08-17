#ifndef DOMAIN_H
#define DOMAIN_H

#include <set>
#include <string>

namespace domain {
const std::string gTableName = "tasks";

class Task {

public:
  Task() : primary_key_(kInActiveKey) { }
  static const int kInActiveKey = -1;

  int get_primary_key() const { return primary_key_; }

private:
  void set_primary_key_(int val) { primary_key_ = val; }
  friend class dal::TaskLifetimeQueries;  // только он меняет первичный ключ
  // Уникальный для каждой задачи
  int primary_key_;  // нужно какое-то не активное

  // Джоел первая.
  // Feature - пока нет
  // Task name
  std::string task_name_;

  // Priority
  int priority;

  // Origin est.
  // Current est.
  // Elapsed
  // Remain

  // TODO: нужно как-то сохранять со связями
  std::set<std::string> tags_;
};
}  // namespace..

#endif // DOMAIN_H
