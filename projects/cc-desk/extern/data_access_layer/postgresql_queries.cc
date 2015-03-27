// Caching
//   http://www.postgresql.org/message-id/E16gYpD-0007KY-00@mclean.mail.mindspring.net
//
// FIXME: писать все запросы буквами одного региста - пусть прописные. это помогает кешировать
//   по крайней мере в MySQL даже регистр важен

#include "heart/config.h"

#include "data_access_layer/postgresql_queries.h"
#include "model_layer/entities.h"
#include "model_layer/filters.h"
#include "common/error_handling.h"

#include <std_own_ext-fix/std_own_ext.h>

#include <iostream>
#include <cassert>
#include <sstream>

namespace storages {
struct TablePositions {
  const static int kId = 0;
  const static int kTaskName = 1;
  const static int kPriority = 2;
  const static int kDone = 3;
};
}

// Unused
//   string sql("SELECT * FROM " + m_table_name + " ORDER BY ID;");

namespace pq_dal {
using namespace storages;

using std::string;
using std::cout;
using std::endl;
using pqxx::connection;
using pqxx::work;
using pqxx::result;
using pqxx::nontransaction;
using entities::Task;
using entities::TaskEntities;
using entities::EntityStates;

using entities::Task;


PostgreSQLDataBase::PostgreSQLDataBase(const std::string& conn_info
                                     , const std::string& table_name)
  : m_conn_ptr(new pqxx::connection(conn_info))
  , m_table_name(table_name)

{
  DCHECK(m_conn_ptr->is_open());
}

PostgreSQLDataBase::~PostgreSQLDataBase() {
  try {
    m_conn_ptr->disconnect();
  } catch (...) {
    // FIXME: add macro what print in debug mode
  }
}

TaskTableQueries PostgreSQLDataBase::getTaskTableQuery() {
  return TaskTableQueries(m_table_name, m_conn_ptr);
}

TaskLifetimeQueries PostgreSQLDataBase::getTaskLifetimeQuery() {
  return TaskLifetimeQueries(m_table_name, m_conn_ptr);
}

void TaskTableQueries::registerBeanClass() {
  string sql(
    "CREATE TABLE " \
    "IF NOT EXISTS "+ // v9.1 >=
    m_table_name +
    "(" \
    // сделать чтобы было >0
    "ID         SERIAL PRIMARY KEY NOT NULL," \
    "TASK_NAME  TEXT               NOT NULL, " \
    "PRIORITY   INT                NOT NULL, " \
    "DONE       BOOLEAN            DEFAULT FALSE);");

  auto c = m_conn_ptr.lock();

  if (!c)
    return;

  work W(*c);
  W.exec(sql);
  W.commit();
}

void TaskTableQueries::drop() {
  auto c = m_conn_ptr.lock();
  if(!c)
    return;

  // Если таблицы нет, то просто ничего не происходит.
  string sql("DROP TABLE " + m_table_name + ";");

  // создаем транзакционный объект
  work w(*c);

  // Exec
  w.exec(sql);
  w.commit();
}

TaskLifetimeQueries::TaskLifetimeQueries(const std::string& table_name
                                         , gc::WeakPtr<pqxx::connection>  p)
    : m_tableName(table_name)
    , m_connPtr(p)
{ }

void TaskLifetimeQueries::update(const entities::Task& v) {

  DCHECK(v.id != EntityStates::kInactiveKey);

  string done("false");
  if (v.done)
    done = "true";

  string sql(
  "UPDATE "
        + m_tableName + " SET "
        + "TASK_NAME = '" + v.name
        + "', PRIORITY = " + std_own_ext::to_string(v.priority)
        + ", DONE = " + done
        + " WHERE ID = " + std_own_ext::to_string(v.id) + ";");
  
  auto c = m_connPtr.lock();
  if (!c)
    return;

  work w(*c);
  w.exec(sql);
  w.commit();
}

entities::Task TaskLifetimeQueries::persist(const entities::Task& task)
{
  DCHECK(task.id == entities::EntityStates::kInactiveKey);
  DCHECK(!task.done);

  string sql(
      "INSERT INTO " + m_tableName + " (TASK_NAME, PRIORITY) " \
        "VALUES ('"
        + task.name + "', "
        + std_own_ext::to_string(task.priority) + ") RETURNING ID; ");

  auto c = m_connPtr.lock();
  if (!c)
    throw std::runtime_error(FROM_HERE);

  work w(*c);
  result r( w.exec( sql ));  // похоже нельзя выполнить два запроса
  w.commit();
  DCHECK(r.size() == 1);

  // Узнаем что за ключ получили
  size_t id(entities::EntityStates::kInactiveKey);
  for (auto c = r.begin(); c != r.end(); ++c) {
    id = c[TablePositions::kId].as<size_t>();
    break;
  }
  DCHECK(id != entities::EntityStates::kInactiveKey);

  // из-за константрости приходится распаковывать значение, нельзя
  //   просто приствоить и оттюнить.
  Task t;
  t.id = id;
  t.name = task.name;
  t.priority = task.priority;
  t.done = false;
  return t;
}


entities::TaskEntities TaskLifetimeQueries::loadAll() const {
  string sql("SELECT * FROM " + m_tableName + ";");// WHERE DONE = FALSE;");

  auto c = m_connPtr.lock();
  if (!c)
    throw std::runtime_error(FROM_HERE);

  work w(*c);
  result r( w.exec( sql ));
  w.commit();

  // pack
  TaskEntities tasks;
  for (auto c = r.begin(); c != r.end(); ++c) {
    Task t;
    t.id = c[TablePositions::kId].as<int>();
    t.name = (c[TablePositions::kTaskName].as<string>());
    t.priority = (c[TablePositions::kPriority].as<int>());
    t.done = (c[TablePositions::kDone].as<bool>());

    auto elem = t.ToEntity();

    tasks.emplace_back(elem);
  }
  return tasks;
}

}  // ns


