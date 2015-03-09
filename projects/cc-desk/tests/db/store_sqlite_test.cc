#include "top/config.h"

#include "wrappers/sqlite_xx.h"
#include "dal/db_indep.h"
#include "dal/sqlite_queries.h"

#include <gtest/gtest.h>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <sqlite3.h>
#include <boost/weak_ptr.hpp>

#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>

std::ostream& operator<<(std::ostream& o, const sqlite3_cc::Result& result) {
  for(auto& row: result) {
    for(auto& column: row) {
      o << column.first << " = " << column.second << " ";
    }
    o  << std::endl;
  }
  return o;
}

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
  const auto result = sqlite3_cc::sqlite3_exec(h, sql);
  std::cout << result;


  sqlite3_cc::sqlite3_exec(h, "drop table COMPANY;");
}

class SQLiteTagTableQuery
{
public:
  void createIfNotExist() : m_table_name("TAGS") {

  }

private:
  const std::string m_table_name;
};

TEST(SQLite, TaskTable) {
  auto h = std::make_shared<sqlite3_cc::sqlite3>("test.db");
  auto table = sqlite_queries::SQLiteTaskTableQueries(h);
  table.createIfNotExist();
}

TEST(SQLite, TagAndTaskTables) {
  auto h = boost::make_shared<sqlite3_cc::sqlite3>("test.db");
  //auto table = SQLiteTagTableQuery(h);
  //table.createIfNotExist();
}
