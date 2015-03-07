#include "top/config.h"

#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>

#include <sqlite3.h>

#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>

namespace sqlite3_cc {
static const int sqlite_ok = SQLITE_OK;

class sqlite3;

std::string sqlite3_errmsg(sqlite3& db_ptr);

// https://www.sqlite.org/threadsafe.html
class sqlite3
{
public:
  ~sqlite3() {
    sqlite3_close(m_db_ptr);
  }

  sqlite3() : m_db_ptr(nullptr) {
    open();
  }

  ::sqlite3* get() {
    return m_db_ptr;
  }


private:
  friend std::string sqlite3_errmsg(sqlite3& db_ptr);
  ::sqlite3* m_db_ptr;

  void open() {
    if( sqlite3_open("test.db", &m_db_ptr) ) {
      throw std::runtime_error(FROM_HERE
                               + std::string("Can't open database: ")
                               + sqlite3_errmsg(*this));
    }
  }

  /**
  */
};

std::string sqlite3_errmsg(sqlite3& db_ptr)
{
  return ::sqlite3_errmsg(db_ptr.m_db_ptr);
}

template <typename R>
R as(const std::string& arg) {
  return boost::lexical_cast<R>(arg);
}
}  // space


static int oneRowHandler(void *NotUsed, int argc, char **argv, char **azColName){
  try {
    auto h = static_cast<std::vector<std::map<std::string, std::string>>*>(NotUsed);
    auto tmp = std::map<std::string, std::string>();
    for (int i = 0; i < argc; ++i) {
      tmp[azColName[i]] = argv[i] ? argv[i] : "NULL";
    }

    h->push_back(tmp);
  } catch(...) { }
  return 0;
}



// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
TEST(SQLiteTest, Base) {
  sqlite3_cc::sqlite3 h;
  std::vector<std::map<std::string, std::string>> processor;

  /* Create SQL statement */
  char* sql = "CREATE TABLE IF NOT EXISTS COMPANY("  \
           "ID INT PRIMARY KEY     NOT NULL," \
           "NAME           TEXT    NOT NULL," \
           "AGE            INT     NOT NULL," \
           "ADDRESS        CHAR(50)," \
           "SALARY         REAL );";

   /* Execute SQL statement */
  sqlite3 *db = h.get();
  char *zErrMsg = 0;
  int rc = sqlite3_exec(db, sql, 0
                        , 0  // !
                        , &zErrMsg);
  if( rc != sqlite3_cc::sqlite_ok ) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  } else {
    fprintf(stdout, "Table created successfully\n");
  }

  //int j = sqlite3_cc::as<int>("123");

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
  // https://www.sqlite.org/c3ref/exec.html
  rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
  if( rc != sqlite3_cc::sqlite_ok ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }else{
    fprintf(stdout, "Records created successfully\n");
  }

  /* Create SQL statement */
  sql = "SELECT * from COMPANY";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, oneRowHandler, (void*)&processor, &zErrMsg);
  if( rc != sqlite3_cc::sqlite_ok ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }else{
    fprintf(stdout, "Operation done successfully\n");
  }

  for(auto& row: processor) {
    for(auto& column: row) {
      std::cout << column.first << " = " << column.second << " ";
    }
    std::cout  << std::endl;
  }
}
