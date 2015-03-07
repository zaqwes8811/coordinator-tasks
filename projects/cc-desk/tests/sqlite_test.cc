#include "top/config.h"

#include <gtest/gtest.h>

#include <sqlite3.h>

// http://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm
TEST(SQLiteTest, Base) {
  sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open("test.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   sqlite3_close(db);
}
