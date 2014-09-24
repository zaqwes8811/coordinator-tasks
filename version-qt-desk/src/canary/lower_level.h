#ifndef DAL_BRIDGE_H
#define DAL_BRIDGE_H

#include <boost/shared_ptr.hpp>
#include <pqxx/pqxx>

#include <string>

namespace lower_level {
namespace psql_space {
using std::string;
using pqxx::connection;
using pqxx::work;

// можно было использовать ссылку и ByRef()
void rm_table(pqxx::connection& C, const std::string& table_name);
void run_transaction(const std::string& sql, pqxx::connection& C);
}
}

#endif // DAL_BRIDGE_H
