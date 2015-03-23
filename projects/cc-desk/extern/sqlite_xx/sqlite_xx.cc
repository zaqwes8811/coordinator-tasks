#include "heart/config.h"

#include "sqlite_xx.h"

#include <sqlite3.h>

#include <vector>
#include <string>
#include <map>

namespace sqlite3_cc {
static const int sqlite_ok = SQLITE_OK;

std::ostream& operator<<(std::ostream& o, const sqlite3_cc::Result& result) {
  for(auto& row: result) {
    for(auto& column: row) {
      o << column.first << " = " << column.second << " ";
    }
    o  << std::endl;
  }
  return o;
}

// https://www.sqlite.org/threadsafe.html
sqlite3::~sqlite3() {
  sqlite3_close(m_db_ptr);
}

sqlite3::sqlite3(const std::string& filename) : m_db_ptr(nullptr) {
  open(filename);
}

void sqlite3::open(const std::string& filename) {
  if( sqlite3_open(filename.c_str(), &m_db_ptr) ) {
    throw std::runtime_error(FROM_HERE
                             + std::string("Can't open database: ")
                             + sqlite3_errmsg(*this));
  }
}


std::string sqlite3_errmsg(sqlite3& db_ptr)
{
  return ::sqlite3_errmsg(db_ptr.m_db_ptr);
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

