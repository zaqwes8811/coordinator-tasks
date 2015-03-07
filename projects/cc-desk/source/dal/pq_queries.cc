// Caching
//   http://www.postgresql.org/message-id/E16gYpD-0007KY-00@mclean.mail.mindspring.net
//
// FIXME: писать все запросы буквами одного региста - пусть прописные. это помогает кешировать
//   по крайней мере в MySQL даже регистр важен

#include "top/config.h"

#include "dal/pq_queries.h"
#include "canary/entities_and_values.h"
#include "canary/filters.h"
#include "top/common.h"
#include "db_indep.h"

#include <boost/bind.hpp>
#include <boost/bind/make_adaptable.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/mem_fn.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <iostream>
#include <cassert>
#include <sstream>

namespace pq_dal {

using namespace boost;
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

PQConnectionPool::PQConnectionPool(const std::string& conn_info)
  : m_conn_ptr(new pqxx::connection(conn_info)) {
  assert(m_conn_ptr->is_open());
}

PQConnectionPool::~PQConnectionPool() {
  try {
    m_conn_ptr->disconnect();
  } catch (...) {}
}


void TaskTableQueries::draw(std::ostream& o) const {
  nontransaction no_tr_w(*m_conn_ptr);
  string sql("SELECT * FROM " + m_table_name + " ORDER BY ID;");
  result r( no_tr_w.exec( sql ));

  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    o << "ID = " << c[TablePositions::kId].as<int>()
         << " Name = " << c[TablePositions::kTaskName].as<string>()
         << " Priority = "  << c[TablePositions::kPriority].as<int>()
         << " Done = "  << c[TablePositions::kDone].as<bool>()
         << endl;
  }
}

void TaskTableQueries::createIfNotExist() {
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

  pq_lower_level::run_transaction(sql, *m_conn_ptr);
}

void TaskTableQueries::drop() {
  pq_lower_level::rm_table(*m_conn_ptr, m_table_name);
}

TaskLifetimeQueries::TaskLifetimeQueries(const std::string& table_name) : m_table_name(table_name)
{ }

void TaskLifetimeQueries::update(const values::ImmutableTask& v, pqxx::connection& C) {

  assert(v.id() != EntitiesStates::kInActiveKey);
  string done("false");
  if (v.done())
    done = "true";

  string sql(
  "UPDATE "
        + m_table_name + " SET "
        + "TASK_NAME = '" + *v.description()
        + "', PRIORITY = " + common::to_string(v.priority())
        + ", DONE = " + done
        + " WHERE ID = " + common::to_string(v.id()) + ";");
  
  work w(C);
  w.exec(sql);
  w.commit();
}

values::ImmutableTask TaskLifetimeQueries::create(const values::ImmutableTask& v, pqxx::connection& conn)
{
  assert(v.id() == entities::EntitiesStates::kInActiveKey);
  assert(!v.done());

  string sql(
      "INSERT INTO " + m_table_name + " (TASK_NAME, PRIORITY) " \
        "VALUES ('"
        + *v.description()+"', "
        + common::to_string(v.priority()) + ") RETURNING ID; ");

  work w(conn);
  result r( w.exec( sql ));  // похоже нельзя выполнить два запроса
  w.commit();
  assert(r.size() == 1);

  // Узнаем что за ключ получили
  int id(entities::EntitiesStates::kInActiveKey);
  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    id = c[TablePositions::kId].as<int>();
    break;
  }
  assert(id != entities::EntitiesStates::kInActiveKey);

  // из-за константрости приходится распаковывать значение, нельзя
  //   просто приствоить и оттюнить.
  return ImmutableTask::create(id, *v.description(), v.priority());
}


entities::Tasks TaskLifetimeQueries::get_all(pqxx::connection& conn) const {
  work w(conn);
  string sql("SELECT * FROM " + m_table_name + ";");// WHERE DONE = FALSE;");
  result r( w.exec( sql ));
  w.commit();

  // pack
  Tasks model;
  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    Tasks::value_type elem = TaskEntity::create("");
    elem->set_primary_key(c[TablePositions::kId].as<int>());
    elem->set_task_name(c[TablePositions::kTaskName].as<string>());
    elem->set_priority(c[TablePositions::kPriority].as<int>());
    elem->set_is_done(c[TablePositions::kDone].as<bool>());

    model.push_back(elem);
  }
  return model;
}

}  // ns

namespace pq_lower_level {
using std::string;
using pqxx::connection;
using pqxx::work;

// можно было использовать ссылку и ByRef()
void rm_table(connection& conn, const string& table_name)
{
  // Если таблицы нет, то просто ничего не происходит.
  string sql("DROP TABLE " + table_name + ";");

  // создаем транзакционный объект
  work w(conn);

  // Exec
  w.exec(sql);
  w.commit();
}

void run_transaction(const string& sql, /*const*/ connection& C)
{
  work W(C);
  W.exec(sql);
  W.commit();
}
}  // ns


