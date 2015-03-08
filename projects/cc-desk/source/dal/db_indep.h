#ifndef DB_INDEP_H_
#define DB_INDEP_H_

#include "canary/entities_and_values.h"

#include <ostream>

namespace storages {
struct TablePositions {
  const static int kId = 0;
  const static int kTaskName = 1;
  const static int kPriority = 2;
  const static int kDone = 3;
};

/**
  \attention Small life time!
*/
class TaskTableQueries
{
public:
  void createIfNotExist()
  { createIfNotExistImpl(); }
  void drop()
  { dropImpl(); }
  void draw(std::ostream& o) const
  { drawImpl(o); }

  virtual ~TaskTableQueries() { }
private:
  virtual void createIfNotExistImpl() = 0;
  virtual void dropImpl() = 0;
  virtual void drawImpl(std::ostream& o) const = 0;
};

/**
  \attention Small life time!

  Делать один репозиторий не советуют

  FIXME: Может DI какой сделать, или все равно?

  FIXME: как то объединить create, update, etc. в persist

  Назначет id!! очень важно! объекты уникальные
  Создает, если еще не был создан, либо обновляет всю запись
  by value
  На групповую вставку могут быть ограничения, но в данной задаче
   пока не нужно, если не нужно будет что-то куда-то автоматически переливать.

  FIXME: с умными указателями возникают проблемы с константростью!
*/
class TaskLifetimeQueries
{
public:
  values::ImmutableTask create(const values::ImmutableTask& v)
  { return createImpl(v); }
  void update(const values::ImmutableTask& v)
  { updateImpl(v); }
  entities::Tasks get_all() const
  { return get_allImpl(); }

  virtual ~TaskLifetimeQueries() { }
private:
  virtual values::ImmutableTask createImpl(const values::ImmutableTask& v) = 0;
  virtual void updateImpl(const values::ImmutableTask& v) = 0;
  virtual entities::Tasks get_allImpl() const = 0;
};
}

#endif
