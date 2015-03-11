// Caching
//   http://www.postgresql.org/message-id/E16gYpD-0007KY-00@mclean.mail.mindspring.net
//
// FIXME: писать все запросы буквами одного региста - пусть прописные. это помогает кешировать
//   по крайней мере в MySQL даже регистр важен

#include "top/config.h"

#include "dal/pq_queries.h"
#include "things/entities.h"
#include "canary/filters.h"
#include "top/error_handling.h"
#include "top/common.h"
#include "db_indep.h"
#include "things/values.h"

#include <iostream>
#include <cassert>
#include <sstream>

namespace pq_dal {
using namespace storages;

using std::string;
using std::cout;
using std::endl;
using pqxx::connection;
using pqxx::work;
using pqxx::result;
using pqxx::nontransaction;
using entities::TaskEntity;
using entities::Tasks;
using entities::EntitiesStates;

using values::ImmutableTask;


PQConnectionsPool::PQConnectionsPool(const std::string& conn_info
                                     , const std::string& table_name)
  : m_conn_ptr(new pqxx::connection(conn_info))
  , m_table_name(table_name)

{
  assert(m_conn_ptr->is_open());
}

PQConnectionsPool::~PQConnectionsPool() {
  try {
    m_conn_ptr->disconnect();
  } catch (...) {
    // FIXME: add macro what print in debug mode
  }
}

std::unique_ptr<storages::TaskTableQueries>
PQConnectionsPool::createTaskTableQueries() {
  return std::unique_ptr<storages::TaskTableQueries>(new TaskTableQueries(m_table_name, m_conn_ptr));
}

std::unique_ptr<storages::TaskLifetimeQueries>
PQConnectionsPool::createTaskLifetimeQueries() {
  return std::unique_ptr<storages::TaskLifetimeQueries>(new TaskLifetimeQueries(m_table_name, m_conn_ptr));
}


void TaskTableQueries::drawImpl(std::ostream& o) const {
  string sql("SELECT * FROM " + m_table_name + " ORDER BY ID;");

  auto c = m_conn_ptr.lock();
  if (!c)
    return;

  nontransaction no_tr_w(*c);
  result r( no_tr_w.exec( sql ));

  for (auto c = r.begin(); c != r.end(); ++c) {
    o << "ID = " << c[TablePositions::kId].as<int>()
         << " Name = " << c[TablePositions::kTaskName].as<string>()
         << " Priority = "  << c[TablePositions::kPriority].as<int>()
         << " Done = "  << c[TablePositions::kDone].as<bool>()
         << endl;
  }
}

void TaskTableQueries::createIfNotExistImpl() {
  string sql(
    "CREATE TABLE " \
    "IF NOT EXISTS "+ // v9.1 >=
    m_table_name +
    "(" \
    // сделать чтобы было >0
    "ID         SERIAL PRIMARY KEY NOT NULL," \
    "TASK_NAME  TEXT               NOT NULL, " \
    "PRIORITY   INT                NOT NULL, " \
    "DONE BOOLEAN DEFAULT FALSE);");

  auto c = m_conn_ptr.lock();

  if (!c)
    return;

  work W(*c);
  W.exec(sql);
  W.commit();
}

void TaskTableQueries::dropImpl() {
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
                                         , app::WeakPtr<pqxx::connection>  p)
    : m_tableName(table_name)
    , m_connPtr(p)
{ }

void TaskLifetimeQueries::updateImpl(const values::ImmutableTask& v) {

  assert(v.id() != EntitiesStates::kInActiveKey);

  string done("false");
  if (v.done())
    done = "true";

  string sql(
  "UPDATE "
        + m_tableName + " SET "
        + "TASK_NAME = '" + *v.description()
        + "', PRIORITY = " + common::to_string(v.priority())
        + ", DONE = " + done
        + " WHERE ID = " + common::to_string(v.id()) + ";");
  
  auto c = m_connPtr.lock();
  if (!c)
    return;

  work w(*c);
  w.exec(sql);
  w.commit();
}

values::ImmutableTask TaskLifetimeQueries::createImpl(const values::ImmutableTask& task)
{
  DCHECK(task.id() == entities::EntitiesStates::kInActiveKey);
  DCHECK(!task.done());

  string sql(
      "INSERT INTO " + m_tableName + " (TASK_NAME, PRIORITY) " \
        "VALUES ('"
        + *task.description()+"', "
        + common::to_string(task.priority()) + ") RETURNING ID; ");

  auto c = m_connPtr.lock();
  if (!c)
    throw std::runtime_error(FROM_HERE);

  work w(*c);
  result r( w.exec( sql ));  // похоже нельзя выполнить два запроса
  w.commit();
  assert(r.size() == 1);

  // Узнаем что за ключ получили
  int id(entities::EntitiesStates::kInActiveKey);
  for (auto c = r.begin(); c != r.end(); ++c) {
    id = c[TablePositions::kId].as<int>();
    break;
  }
  assert(id != entities::EntitiesStates::kInActiveKey);

  // из-за константрости приходится распаковывать значение, нельзя
  //   просто приствоить и оттюнить.
  return ImmutableTask::create(id, *task.description(), task.priority());
}


entities::Tasks TaskLifetimeQueries::get_allImpl() const {
  string sql("SELECT * FROM " + m_tableName + ";");// WHERE DONE = FALSE;");

  auto c = m_connPtr.lock();
  if (!c)
    throw std::runtime_error(FROM_HERE);

  work w(*c);
  result r( w.exec( sql ));
  w.commit();

  // pack
  Tasks model;
  for (auto c = r.begin(); c != r.end(); ++c) {
    auto elem = TaskEntity::create("");
    elem->set_primary_key(c[TablePositions::kId].as<int>());
    elem->set_task_name(c[TablePositions::kTaskName].as<string>());
    elem->setPriority(c[TablePositions::kPriority].as<int>());
    elem->set_is_done(c[TablePositions::kDone].as<bool>());

    model.push_back(elem);
  }
  return model;
}

}  // ns


