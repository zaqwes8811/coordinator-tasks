#include "tests_config.h"
//#include "arrays.h"

// C++
#include <string>

// Other
#include <v8-r16258/include/v8.h>
#include <gtest/gtest.h>

// App
#include "process.h"

using namespace std;
using namespace v8;


TEST(ArrayAuto, Base) {
  v8::V8::InitializeICU();
  string file = string("scripts/arrays.js");
  EXPECT_NE(true, file.empty());

  // Isolate and context be injected!
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope handle_scope(isolate);

  // Script
  Handle<String> source = ReadFile(file);
  ASSERT_NE(true, source.IsEmpty());

  // V8 space
  Handle<ObjectTemplate> global = ObjectTemplate::New();
  global->Set(String::New("log"), FunctionTemplate::New(LogCallback));

  v8::Handle<v8::Context> context = 
        Context::New(isolate, NULL, global);

  Context::Scope context_scope(context);

  // Our objs
  //DataBase database;
  //database.unlocks_[0] = 10;

  //@Point
  // Install
  //Handle<Object> output_obj = V8DataBase::New(&database, isolate);

  //context->Global()->Set(String::New("database"), output_obj);

  //@Point
  // Run Script
  //bool success = ExecuteScript(source, isolate);
}
