#include <typeinfo>

// C
#include <cassert>

// C++
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>

// 3rdparty::Common
#include <loki/ScopeGuard.h>

// 3rdparty::Special
#include <pqxx/pqxx> 

using namespace std;
using namespace pqxx;
using namespace Loki;

namespace {
const string gTableName = "tasks";

class Task {
 
public:
  Task() : primary_key_(kInActiveKey) { }
  static const int kInActiveKey = -1;
  
private:
  // Уникальный для каждой задачи
  int primary_key_;  // нужно какое-то не активное
  
  // Джоел первая.
  // Feature - пока нет
  // Task name
  string task_name_;
  
  // Priority
  int priority;
  
  // Origin est.
  // Current est.
  // Elapsed
  // Remain
  
  // TODO: нужно как-то сохранять со связями
  std::set<std::string> tags_;
};

namespace dal {
// можно было использовать ссылку и ByRef()
void rm_table(connection& C, const string& table_name) 
{
  // Если таблицы нет, то просто ничего не происходит.
  string sql = string("drop table " + table_name + ";");
  
  // создаем транзакционный объект
  work W(C);
  
  // Exec
  W.exec(sql);
  W.commit();
}

void run_transaction(const string& sql, /*const*/ connection& C)  
{
  work W(C);
  W.exec(sql);
  W.commit();
}

class TaskTableQueries {
public:
  TaskTableQueries(const string& name) : table_name_(name) { }
  
  void createTable(connection& C) {
    string sql = string("CREATE TABLE ") +  
      "IF NOT EXISTS "+ // v9.1 >=
      table_name_ +
      "(" \
      "id SERIAL PRIMARY KEY NOT NULL," \
      "task_name  TEXT NOT NULL, " \
      "priority INT NOT NULL);";
    
    run_transaction(sql, C);
  }

  void drop_task_table(connection& C) {
    rm_table(C, table_name_);
  }
  
private:
  const string table_name_;
  
};

// Делать один репозиторий не советуют
// TODO: Может DI какой сделать, или все равно?
class TaskLifetimeQueries {
public:
  explicit TaskLifetimeQueries(const string& table_name) : table_name_(table_name) {}
  void persist(const Task& task) const {
    
  }
  
  // Назначет id!
  Task& store(Task& task, connection& C) const { 
    // нужно получить id
    // http://stackoverflow.com/questions/2944297/postgresql-function-for-last-inserted-id
    {
      // Insert
      string sql = 
	"INSERT INTO " + table_name_ + " (task_name, priority) " \
	  "VALUES ('Paul', 32) RETURNING id; ";
      
      work W(C);
      result R( W.exec( sql ));
      W.commit();
      
      /* List down all the records */
      int current_id = Task::kInActiveKey;
      for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
	current_id = c[0].as<int>();
	break;
      }
      
      assert(current_id != Task::kInActiveKey);
      
      //task
    }
    return task; 
  }
  
  //void removeById(int id);

private:
  TaskLifetimeQueries(const TaskLifetimeQueries&);
  TaskLifetimeQueries& operator=(const TaskLifetimeQueries&);
  
  const string table_name_;
};

}  // namespace..
}  // namespace..

int main() {
  using namespace dal;
  try {
  // construction is safe? 
    connection C("dbname=mydb user=postgres password=postgres hostaddr=127.0.0.1 port=5432");
    {
      // DANGER: Рефакторинг мог быть не эквивалентным, но в api сказано, что закрывает только откр. соед.
      if (!C.is_open()) {
	//guard.Dismiss();  // not need it
	throw runtime_error("Can't open database");
      }
      
      ScopeGuard guard = MakeObjGuard(C, &connection::disconnect);
      
      /// Work
      {
	const string kTableName = "task_fake_entity";
	TaskTableQueries q(kTableName);
	q.createTable(C);
	
	// Если не создано, то нет смысла
	ScopeGuard table_guard = MakeObjGuard(q, &TaskTableQueries::drop_task_table, ByRef(C));  // а если не создасться? Тут похоже все равно.
	
	/// Create records
	TaskLifetimeQueries q_insert(kTableName);
	Task t;
	q_insert.store(t, C);
	q_insert.store(t, C);
	
	/// View
	/* Create a non-transactional object. */
	nontransaction N(C);
	
	/* Execute SQL query */
	string sql = "SELECT * from " + kTableName + ";";
	result R( N.exec( sql ));
	
	/* List down all the records */
	for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
	  cout << "ID = " << c[0].as<int>() << " Name = " << c[1].as<string>() << " Priority = " << c[2].as<int>() << endl;
	}
	
	
	// Лучше здесь - раз дошли до сюда, то таблица создана
	//ScopeGuard table = MakeGuard(&rm_table, &C);

      }
    }

    // POINT - disconnected
    assert(!C.is_open());
  } catch (const pqxx::undefined_table& e) {
    // Нет таблицы
    cerr << typeid(e).name() << endl << e.what() << endl;
  } catch (const pqxx::unique_violation& e) {
    // Не уникальный ключ при вствке
    cerr << typeid(e).name() << endl << e.what() << endl;
  } catch (const pqxx::sql_error& e) {
    // Кажется ошибка в синтаксисе
    cerr << typeid(e).name() << endl << e.what() << endl;
  } catch (const std::exception& e) {
    cerr << "UNCATCHED" << endl;
    cerr << typeid(e).name() << endl;
    cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}