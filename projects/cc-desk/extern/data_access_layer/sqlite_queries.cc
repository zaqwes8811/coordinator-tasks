#include "heart/config.h"

#include "sqlite_queries.h"

#include <string>

namespace sqlite_queries {
using std::string;
using entities::Task;

entities::Task SQLiteTaskTableQueries::persist(const entities::Task& unsaved_task) {
  using sqlite3_cc::as;

  DCHECK(unsaved_task.id == entities::EntityStates::kInactiveKey);

  string sql(
      "INSERT INTO " + m_tableName + " (TASK_NAME, PRIORITY) " \
      "VALUES ('" + unsaved_task.name+"','" + std_own_ext::to_string(unsaved_task.priority) + "'); " +
      "  SELECT last_insert_rowid() FROM " + m_tableName + "; ");

  auto c = m_connPtr.lock();
  if (!c)
    throw std::runtime_error(FROM_HERE);

  // query
  auto r = sqlite3_cc::sqlite3_exec(*c, sql);
  DCHECK(r.size() == 1);

  // Узнаем что за ключ получили
  size_t id(entities::EntityStates::kInactiveKey);
  for (auto& col : r) {
    id = as<size_t>(col["ID"]);
    break;
  }
  DCHECK(id != entities::EntityStates::kInactiveKey);

  auto saved_task = Task();
  saved_task.id = id;
  saved_task.name = unsaved_task.name;
  saved_task.priority = unsaved_task.priority;
  saved_task.done = false;
  return saved_task;
}
void SQLiteTaskTableQueries::update(const entities::Task& v) { }

entities::TaskEntities SQLiteTaskTableQueries::loadAll() const {
  return entities::TaskEntities();
}

SQLiteTaskTableQueries::SQLiteTaskTableQueries(gc::WeakPtr<sqlite3_cc::sqlite3> h
                                               , const std::string& tableName)
  : m_tableName(tableName)
  , m_connPtr(h) { }

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

  auto c = m_connPtr.lock();

  if (!c)
    return;

  sqlite3_cc::sqlite3_exec(*c, sql);
}

void SQLiteTaskTableQueries::drop() {
  auto c = m_connPtr.lock();

  if (!c)
    return;

  sqlite3_cc::sqlite3_exec(*c, "DROP TABLE " + m_tableName + ";");
}

bool checkUnique(const std::string& name, gc::WeakPtr<sqlite3_cc::sqlite3> h) {
  auto c = h.lock();
  if (!c) return false;

  auto sql = "SELECT NAME FROM " + string(models::s_kTagTableName) + " WHERE NAME ='" + name + "';";
  auto r = sqlite3_cc::sqlite3_exec(*c, sql);
  return r.empty();
}

entities::TagEntity createTag(const entities::Tag& tag, gc::WeakPtr<sqlite3_cc::sqlite3> h) {
  using sqlite3_cc::as;

  DCHECK(tag.id == entities::EntityStates::kInactiveKey);
  DCHECK(checkUnique(tag.name, h));

  // http://stackoverflow.com/questions/531109/how-to-return-the-value-of-auto-increment-column-in-sqlite-with-vb6
  string sql(
      "INSERT INTO " + string(models::s_kTagTableName) + " (NAME, COLOR) " \
      "VALUES ('" + tag.name+"','" + tag.color + "'); " \
      "  SELECT last_insert_rowid() FROM " + models::s_kTagTableName + "; ");

  auto c = h.lock();
  if (!c)
    throw std::runtime_error(FROM_HERE);

  // query
  auto r = sqlite3_cc::sqlite3_exec(*c, sql);
  DCHECK(r.size() == 1);

  // Узнаем что за ключ получили
  size_t id(entities::EntityStates::kInactiveKey);
  for (auto& col : r) {
    id = as<size_t>(col["ID"]);
    break;
  }
  DCHECK(id != entities::EntityStates::kInactiveKey);

  auto entity = std::make_shared<entities::Tag>();
  entity->id = id;
  entity->name = tag.name;
  entity->color = tag.color;

  return entity;
}


SQLiteTagTableQuery::SQLiteTagTableQuery(gc::WeakPtr<sqlite3_cc::sqlite3> h)
    : m_tableName(models::s_kTagTableName)
    , m_connPtr(h) { }

void SQLiteTagTableQuery::registerBeanClass()  {
  // http://www.tutorialspoint.com/sqlite/sqlite_using_autoincrement.htm
  std::string sql = "CREATE TABLE IF NOT EXISTS " + m_tableName + "("  \
           "ID             INTEGER  PRIMARY KEY     AUTOINCREMENT," \
           "NAME           TEXT                    NOT NULL," \
           "COLOR          TEXT                    NOT NULL);";

  auto c = m_connPtr.lock();
  if (!c) return;
  sqlite3_cc::sqlite3_exec(*c, sql);
}

void SQLiteTagTableQuery::drop() {
  auto c = m_connPtr.lock();
  if (!c) return;
  sqlite3_cc::sqlite3_exec(*c, "DROP TABLE " + m_tableName + ";");
}
}  // space
