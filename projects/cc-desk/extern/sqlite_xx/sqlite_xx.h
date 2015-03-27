#ifndef WR_SQLITE_XX_H_
#define WR_SQLITE_XX_H_

#include <std_own_ext-fix/std_own_ext.h>
#include <sqlite3.h>

#include <string>
#include <vector>
#include <map>

namespace sqlite3_cc {
typedef std::vector<std::map<std::string, std::string>> Result;
static const char* const null_value = "NULL";

template <typename R>
R as(const std::string& arg) {
  return std_own_ext::lexical_cast<R>(arg);
}

class sqlite3
{
public:
  ~sqlite3();

  explicit sqlite3(const std::string& filename);

private:
  friend std::string sqlite3_errmsg(sqlite3& db_ptr);
  friend Result sqlite3_exec(sqlite3& db_ptr, const std::string& sql);
  ::sqlite3* m_db_ptr;
  void open(const std::string& filename);
};

std::string sqlite3_errmsg(sqlite3& db_ptr);
Result sqlite3_exec(sqlite3& db_ptr, const std::string& sql);

std::ostream& operator<<(std::ostream& o, const sqlite3_cc::Result& result);

}  // space

#endif
