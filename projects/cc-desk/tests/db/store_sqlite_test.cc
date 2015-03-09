#include "top/config.h"

#include "wrappers/sqlite_xx.h"
#include "dal/db_indep.h"

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <sqlite3.h>

#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
TEST(SQLiteTest, Base) {
  sqlite3_cc::sqlite3 h("test.db");


  /* Create SQL statement */
  char* sql = "CREATE TABLE IF NOT EXISTS COMPANY("  \
           "ID INT PRIMARY KEY     NOT NULL," \
           "NAME           TEXT    NOT NULL," \
           "AGE            INT     NOT NULL," \
           "ADDRESS        CHAR(50)," \
           "SALARY         REAL );";

   /* Execute SQL statement */
  sqlite3_cc::sqlite3_exec(h, sql);

  /* Create SQL statement */
  sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
       "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
       "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
       "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
       "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
       "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
       "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
       "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

  /* Execute SQL statement */
  sqlite3_cc::sqlite3_exec(h, sql);

  /* Create SQL statement */
  sql = "SELECT * from COMPANY";

  /* Execute SQL statement */
  sqlite3_cc::Result result = sqlite3_cc::sqlite3_exec(h, sql);
  for(auto& row: result) {
    for(auto& column: row) {
      std::cout << column.first << " = " << column.second << " ";
    }
    std::cout  << std::endl;
  }

  sqlite3_cc::sqlite3_exec(h, "drop table COMPANY;");
}


class SQLiteTaskTableQueries : public storages::TaskTableQueries
{
public:
  explicit TaskTableQueries(boost::weak_ptr<sqlite3_cc::sqlite3> h)
    : m_table_name()
    , m_conn_ptr(h) { }

private:
  virtual void createIfNotExistImpl() {

  }
  virtual void dropImpl() {

  }
  virtual void drawImpl(std::ostream& o) const {

  }

  const std::string m_table_name;
  boost::weak_ptr<sqlite3_cc::sqlite3> m_conn_ptr;
};


TEST(SQLite, TasksTable) {
  auto h = boost::make_shared<sqlite3_cc::sqlite3>
}
