#ifndef DB_INDEP_H_
#define DB_INDEP_H_

#include "model_layer/entities.h"

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
  void createIfNotExist();
  void drop();
  void draw(std::ostream& o) const;

  virtual ~TaskTableQueries() { }
private:
  virtual void createIfNotExistImpl() = 0;
  virtual void dropImpl() = 0;
  virtual void drawImpl(std::ostream& o) const = 0;

  virtual std::string getTableName() const = 0;
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
  entities::TaskValue create(const entities::TaskValue& v);
  void update(const entities::TaskValue& v);
  entities::TaskEntities get_all() const;

  virtual ~TaskLifetimeQueries() { }
private:
  virtual entities::TaskValue createImpl(const entities::TaskValue& v) = 0;
  virtual void updateImpl(const entities::TaskValue& v) = 0;
  virtual entities::TaskEntities get_allImpl() const = 0;
};

/**
  \attention Only in single thread! Actors Model can help

  http://herbsutter.com/2013/05/30/gotw-90-solution-factories/

  \fixme How make noncopyble in C++11
*/
class DataBaseDriver
    //: public boost::noncopyable
{
public:
  virtual ~DataBaseDriver() { }

  /**
    \fixme: strange design. May be bad lifetimes

    http://www.drdobbs.com/cpp/c11-uniqueptr/240002708
  */
  virtual std::unique_ptr<storages::TaskTableQueries> createTaskTableQuery() = 0;

  virtual std::unique_ptr<storages::TaskLifetimeQueries> createTaskLifetimeQuery() = 0;

  //DataBaseDriver(const DataBaseDriver&) = delete;
};


typedef app::SharedPtr<storages::DataBaseDriver> DataBaseDriverPtr;
}

#endif
