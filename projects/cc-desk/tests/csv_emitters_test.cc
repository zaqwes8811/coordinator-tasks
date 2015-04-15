#include "heart/config.h"

#include <data_access_layer/sqlite_queries.h>
#include <gtest/gtest.h>
#include <sqlite_xx/sqlite_xx.h>

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

std::ostream& operator<<(std::ostream& o, const sqlite3_cc::Result& result) {
  for(auto& row: result) {
    for(auto& column: row) {
      o << column.first << " = " << column.second << " ";
    }
    o  << std::endl;
  }
  return o;
}

string make_text(string s) { return "\"" + s + "\""; }

string join(vector<string> v, string by) {
  if (v.empty()) return "";
  if (v.size() == 1) return v[0];

  auto except_last = vector<string>(v.begin(), --v.end());

  string r;
  for (auto & a: except_last)
    r.append(make_text(a)).append(by);

  r.append(make_text(v.back()));
  return r;
}

TEST(EmitterApp, SQLite) {
  auto h = sqlite3_cc::sqlite3("./test.db");

  auto r = sqlite3_cc::sqlite3_exec(h, string("select * from ") + models::kTaskTableNameRef + ";");

  ofstream f("sqlite_v0.csv");

  EXPECT_TRUE(f.is_open());

  f << join(vector<string>{"Feature", "Task", "Priority", "Done"}, ", ");
  f << endl;

  for (auto& col : r) {
    f << join(vector<string>{"", col["TASK_NAME"], col["PRIORITY"], col["DONE"]}, ", ");
    f << endl;
  }
}
