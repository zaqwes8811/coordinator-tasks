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
typedef std::vector<std::map<std::string, std::string>> Result;
static const int sqlite_ok = SQLITE_OK;
const char* const null_value = "NULL";

class sqlite3;

std::string sqlite3_errmsg(sqlite3& db_ptr);
Result sqlite3_exec(sqlite3& db_ptr, const std::string& sql);

// https://www.sqlite.org/threadsafe.html
class sqlite3
{
public:
  ~sqlite3() {
    sqlite3_close(m_db_ptr);
  }

  explicit sqlite3(const std::string& filename) : m_db_ptr(nullptr) {
    open(filename);
  }

  ::sqlite3* get() {
    return m_db_ptr;
  }


private:
  friend std::string sqlite3_errmsg(sqlite3& db_ptr);
  friend Result sqlite3_exec(sqlite3& db_ptr, const std::string& sql);

  ::sqlite3* m_db_ptr;

  void open(const std::string& filename) {
    if( sqlite3_open(filename.c_str(), &m_db_ptr) ) {
      throw std::runtime_error(FROM_HERE
                               + std::string("Can't open database: ")
                               + sqlite3_errmsg(*this));
    }
  }
};

std::string sqlite3_errmsg(sqlite3& db_ptr)
{
  return ::sqlite3_errmsg(db_ptr.m_db_ptr);
}

template <typename R>
R as(const std::string& arg) {
  return boost::lexical_cast<R>(arg);
}

static int one_row_handler(void *targetPtr, int argc, char **argv, char **azColName){
  try {
    if (argc == 0)
      return 0;

    auto h = static_cast<Result*>(targetPtr);
    auto tmp = Result::value_type();
    for (int i = 0; i < argc; ++i) {
      tmp[azColName[i]] = argv[i] ? argv[i] : null_value;
    }

    h->push_back(std::move(tmp));
  } catch(...) { }
  return 0;
}

Result sqlite3_exec(sqlite3& db_ptr, const std::string& sql)
{
  // https://www.sqlite.org/c3ref/exec.html
  Result r;
  char *zErrMsg = 0;
  auto rc = ::sqlite3_exec(db_ptr.m_db_ptr
                          , sql.c_str()
                          , &sqlite3_cc::one_row_handler
                          , (void*)&r, &zErrMsg);
  if( rc != sqlite3_cc::sqlite_ok ){
    auto msg = "SQL error: " + std::string(zErrMsg);
    sqlite3_free(zErrMsg);
    throw std::runtime_error(FROM_HERE + msg);
  }
  return r;
}

}  // space


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
