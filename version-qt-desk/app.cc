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
void rm_table(connection* C, const string& table_name) 
{
  // Если таблицы нет, то просто ничего не происходит.
  string sql = string("drop table " + table_name + ";");
  
  // создаем транзакционный объект
  work W(*C);
  
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
  
  void create_task_table(connection& C) {
    // TODO: create only if not exist - с исключением не катит.
    // TODO: а кто вообще создает таблицы? Всегда ли пользователь?
    //
    // TODO: как безболезненной сделать изменение схемы хранения? Как делать рефакторинг?
    string sql = string("CREATE TABLE ") +  
      // http://stackoverflow.com/questions/1766046/postgresql-create-table-if-not-exists
      "IF NOT EXISTS "+ // v9.1 >=
      table_name_ +
      "(" \
      // http://www.tutorialspoint.com/postgresql/postgresql_using_autoincrement.htm
      //"ID INT PRIMARY KEY NOT NULL," \  // own control
      "ID SERIAL PRIMARY KEY NOT NULL," \
      "task_name  TEXT NOT NULL, " \
      "priority INT NOT NULL);";
    
    // создаем транзакционный объект
    run_transaction(sql, C);
  }

  void drop_task_table(connection& C) {
    rm_table(&C, table_name_);
  }
  
private:
  const string table_name_;
  
};

// Делать один репозиторий не советуют
// TODO: Может DI какой сделать, или все равно?
class TaskLifetimeQueries {
public:
  void persist(const Task& task) const {
    
  }
  
  // Назначет id!
  Task& create(Task& task) const { 
    	
    /*
    {
      // Insert
      // http://stackoverflow.com/questions/7718585/set-auto-increment-primary-key-in-postgresql
      //string sql = "INSERT INTO COMPANY (ID, NAME, AGE, ADDRESS, SALARY) " \
      //"VALUES (1, 'Paul', 32, 'Calif', 200000.00); ";
      
      string sql = "INSERT INTO " + gTableName + " (NAME, AGE, ADDRESS, SALARY) " \
      "VALUES ('Paul', 32, 'Calif', 200000.00); " \
      "VALUES ('Djudy', 32, 'Calif', 900000.00); ";
      
      run_transaction(sql, C);
    }
    */
    return task; 
  }
  
  //void removeById(int id);
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
	TaskTableQueries q("task_fake_entity");
	ScopeGuard table = MakeObjGuard(q, &TaskTableQueries::drop_task_table, ByRef(C));  // а если не создасться? Тут похоже все равно.
	
	
	
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