#ifndef TOP_CONFIG_H_
#define TOP_CONFIG_H_

//#define G_I_WANT_USE_IT  // external
#define G_USE_PSQL

#include "common/error_handling.h"

namespace models {
const char* const kConnection = "dbname=mydb user=postgres password=apassword hostaddr=127.0.0.1 port=5432";
#ifdef G_I_WANT_USE_IT
const char* const kTaskTableNameRef = "task_entity";
#else
const char* const kTaskTableNameRef = "task_fake_entity";
#endif

const int kAddedBlankLines = 10;
}
#endif   
