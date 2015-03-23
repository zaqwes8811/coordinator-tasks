#include "heart/config.h"

#include "sqlite_queries.h"

#include <string>
#include <iostream>

namespace sqlite_queries {
using std::string;
using entities::Task;
using entities::TaskEntities;
using sqlite3_cc::as;
using sqlite3_cc::sqlite3_exec;
using std_own_ext::at;
using entities::Tag;

// http://stackoverflow.com/questions/27764486/java-sqlite-last-insert-rowid-return-0
size_t get_last_id(const sqlite3_cc::Result& r) {
  // FIXME: Strange thing. Really return bunch identical rows.
  //DCHECK(r.size() == 1);

  size_t id(entities::EntityStates::kInactiveKey);
  for (auto& col : r) {
    id = as<size_t>(at(col, "last_insert_rowid()"));
    break;
  }

  DCHECK(id != entities::EntityStates::kInactiveKey);
  return id;
}

Task SQLiteTaskTableQueries::persist(const entities::Task& unsaved_task) {
  DCHECK(unsaved_task.id == entities::EntityStates::kInactiveKey);

  string sql(
      "INSERT INTO " + m_tableName + " (TASK_NAME, PRIORITY) " \
      "VALUES ('" + unsaved_task.name+"','"
      + std_own_ext::to_string(unsaved_task.priority) + "'); " +
      "  SELECT last_insert_rowid() FROM " + m_tableName + "; ");

  auto r = exec(sql);

  auto saved_task = Task();
  saved_task.id = get_last_id(r);
  saved_task.name = unsaved_task.name;
  saved_task.priority = unsaved_task.priority;
  saved_task.done = false;
  return saved_task;
}

void SQLiteTaskTableQueries::update(const entities::Task& v) {
  DCHECK(v.id != entities::EntityStates::kInactiveKey);

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

  exec(sql);
}

TaskEntities SQLiteTaskTableQueries::loadAll() const {
  string sql("SELECT * FROM " + m_tableName + ";");

  auto r = exec(sql);

  TaskEntities tasks;
  for (auto& col : r) {
    Task t;
    t.id = as<size_t>(col["ID"]);
    t.name = as<string>(col["TASK_NAME"]);
    t.priority = as<int>(col["PRIORITY"]);
    t.done = as<bool>(col["DONE"]);

    tasks.emplace_back(t.share());
  }
  return tasks;
}

SQLiteTaskTableQueries::SQLiteTaskTableQueries(gc::WeakPtr<sqlite3_cc::sqlite3> h
                                               , const std::string& tableName)
  : m_tableName(tableName), m_connPtr(h)
{ }

void SQLiteTaskTableQueries::registerBeanClass() {
  std::string sql(
    "CREATE TABLE " \
    "IF NOT EXISTS "+
    m_tableName +
    "(" \
    "ID         SERIAL PRIMARY KEY NOT NULL," \
    "TASK_NAME  TEXT               NOT NULL, " \
    "PRIORITY   INT                NOT NULL, " \
    "DONE       BOOLEAN            DEFAULT FALSE);");

  exec(sql);
}

void SQLiteTaskTableQueries::drop() { exec("DROP TABLE " + m_tableName + ";"); }

bool SQLiteTagTableQuery::checkUnique(const std::string& name) {
  auto sql = "SELECT NAME FROM " + m_tableName + " WHERE NAME ='" + name + "';";
  return exec(sql).empty();
}

Tag SQLiteTagTableQuery::persist(const Tag& unsaved_tag) {
  using sqlite3_cc::operator <<;

  DCHECK(unsaved_tag.id == entities::EntityStates::kInactiveKey);
  DCHECK(checkUnique(unsaved_tag.name));

  string sql(
      "INSERT INTO " + m_tableName + " (NAME, COLOR) " \
      "VALUES ('" + unsaved_tag.name+"','" + unsaved_tag.color + "'); " \
      "  SELECT last_insert_rowid() FROM " + m_tableName + "; ");

  auto r = exec(sql);
  std::cout << r;

  auto saved_tag = Tag();
  saved_tag.id = get_last_id(r);
  saved_tag.name = unsaved_tag.name;
  saved_tag.color = unsaved_tag.color;

  return saved_tag;
}


SQLiteTagTableQuery::SQLiteTagTableQuery(gc::WeakPtr<sqlite3_cc::sqlite3> h)
    : m_tableName(models::s_kTagTableName) , m_connPtr(h)
{ }

void SQLiteTagTableQuery::registerBeanClass()  {
  // http://www.tutorialspoint.com/sqlite/sqlite_using_autoincrement.htm
  std::string sql = "CREATE TABLE IF NOT EXISTS " + m_tableName + "("  \
           "ID             INTEGER  PRIMARY KEY     AUTOINCREMENT," \
           "NAME           TEXT                    NOT NULL," \
           "COLOR          TEXT                    NOT NULL);";

  exec(sql);
}

void SQLiteTagTableQuery::drop() {
  exec("DROP TABLE " + m_tableName + ";");
}
}  // space
