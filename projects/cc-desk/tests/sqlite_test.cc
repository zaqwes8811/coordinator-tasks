#include "top/config.h"

#include <gtest/gtest.h>

#include <sqlite3.h>

#include <string>
#include <stdexcept>

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

}  // space

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
TEST(SQLiteTest, Base) {
  sqlite3_cc::sqlite3 h;
}
