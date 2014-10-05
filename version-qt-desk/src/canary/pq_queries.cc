// Caching
//   http://www.postgresql.org/message-id/E16gYpD-0007KY-00@mclean.mail.mindspring.net
//
// FIXME: писать все запросы буквами одного региста - пусть прописные. это помогает кешировать
//   по крайней мере в MySQL даже регистр важен

#include "top/config.h"

#include "canary/pq_queries.h"
#include "canary/entities_and_values.h"
#include "canary/filters.h"
#include "top/common.h"

#include <boost/bind.hpp>
#include <boost/bind/make_adaptable.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/mem_fn.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <adobe/algorithm/partition.hpp>

#include <iostream>
#include <cassert>
#include <sstream>

namespace pq_dal {
struct TablePositions {
  const static int kId = 0;
  const static int kTaskName = 1;
  const static int kPriority = 2;
  const static int kDone = 3;
};

using namespace boost;

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

PQConnectionPool::PQConnectionPool(const std::string& conn_info)
  : conn_(new pqxx::connection(conn_info)) {
  assert(conn_->is_open());
}

PQConnectionPool::~PQConnectionPool() {
  try {
    conn_->disconnect();
  } catch (...) {}
}


void TaskTableQueries::print(std::ostream& o, connection& conn) const {
  nontransaction no_tr_w(conn);
  string sql("SELECT * FROM " + table_name_ + " ORDER BY ID;");
  result r( no_tr_w.exec( sql ));

  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    o << "ID = " << c[TablePositions::kId].as<int>()
         << " Name = " << c[TablePositions::kTaskName].as<string>()
         << " Priority = "  << c[TablePositions::kPriority].as<int>()
         << " Done = "  << c[TablePositions::kDone].as<bool>()
         << endl;
  }
}

void TaskTableQueries::createIfNotExist(connection& C) {
  string sql(
    "CREATE TABLE " \
    "IF NOT EXISTS "+ // v9.1 >=
    table_name_ +
    "(" \
    // сделать чтобы было >0
    "ID         SERIAL PRIMARY KEY NOT NULL," \
    "TASK_NAME  TEXT               NOT NULL, " \
    "PRIORITY   INT                NOT NULL, " \
    "DONE BOOLEAN DEFAULT FALSE);");

  pq_lower_level::run_transaction(sql, C);
}

void TaskTableQueries::drop(connection& C) {
  pq_lower_level::rm_table(C, table_name_);
}

void TaskLifetimeQueries::update(entities::Tasks::value_type e, pqxx::connection& C) {
  assert(e->get_primary_key() != EntitiesStates::kInActiveKey);
  string done("false");
  if (e->get_is_done())
    done = "true";

  string sql(
  "UPDATE "
        + task_table_name_ + " SET "
        + "TASK_NAME = '" + e->get_task_name()
        + "', PRIORITY = " + common::to_string(e->get_priority())
        + ", DONE = " + done
        + " WHERE ID = " + common::to_string(e->get_primary_key()) + ";");
  
  work w(C);
  w.exec(sql);
  w.commit();
}

void TaskLifetimeQueries::create(entities::Tasks::value_type task, pqxx::connection& C) {
  create(*task, C);
}

void TaskLifetimeQueries::create(
    entities::Tasks::value_type::element_type& e, connection& conn) {
  // нужно получить id
  // http://stackoverflow.com/questions/2944297/postgresql-function-for-last-inserted-id
  string sql(
      "INSERT INTO " + task_table_name_ + " (TASK_NAME, PRIORITY) " \
        "VALUES ('"
        + e.get_task_name()+"', "
        + common::to_string(e.get_priority()) + ") RETURNING ID; ");

  work w(conn);
  result r( w.exec( sql ));  // похоже нельзя выполнить два запроса
  w.commit();

  // Узнаем что за ключ получили
  int new_id(entities::EntitiesStates::kInActiveKey);

  assert(r.size() == 1);  // вставили один элемент
  
  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    new_id = c[TablePositions::kId].as<int>();
    break;
  }

  assert(new_id != entities::EntitiesStates::kInActiveKey);
  e.set_primary_key_(new_id);
}

entities::Tasks TaskLifetimeQueries::_pack(pqxx::result& r) {
  Tasks model;
  for (result::const_iterator c = r.begin(); c != r.end(); ++c) {
    Tasks::value_type elem = TaskEntity::create("");
        //Model::value_type::element_type::create(string());
    elem->set_primary_key_(c[TablePositions::kId].as<int>());
    elem->set_task_name(c[TablePositions::kTaskName].as<string>());
    elem->set_priority(c[TablePositions::kPriority].as<int>());
    elem->set_is_done(c[TablePositions::kDone].as<bool>());

    model.push_back(elem);
  }
  return model;
}

entities::Tasks TaskLifetimeQueries::get_all(pqxx::connection& conn) const {
  work w(conn);
  string sql("SELECT * FROM " + task_table_name_ + ";");// WHERE DONE = FALSE;");
  result r( w.exec( sql ));
  w.commit();
  return _pack(r);
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


