#include "tests_config.h"

// C++
#include <string>
#include <map>

// Other
#include <v8-r16258/include/v8.h>
#include <gtest/gtest.h>

// App
#include "process.h"
#include "point_v8.h"
#include "v8_wrapper.h"
#include "virtual_machine.h"

using std::string;

TEST(V8, ProcessTop) {
  const int kSampleSize = 6;
  StringHttpRequest kSampleRequests[kSampleSize] = {
    StringHttpRequest("/process.cc", "localhost", "google.com", "firefox"),
    StringHttpRequest("/", "localhost", "google.net", "firefox"),
    StringHttpRequest("/", "localhost", "google.org", "safari"),
    StringHttpRequest("/", "localhost", "yahoo.com", "ie"),
    StringHttpRequest("/", "localhost", "yahoo.com", "safari"),
    StringHttpRequest("/", "localhost", "yahoo.com", "firefox")
  };
  int argc = 2;
  string path = kPathToTestScripts+string("./scripts/count-hosts.js");
  const char* p = path.c_str();
  const char* argv[] = {"", p};

  v8::V8::InitializeICU();
  map<string, string> options;
  string file;
  ParseOptions(argc, argv, options, &file);
  ASSERT_NE(true, file.empty());

  //> V8
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Handle<String> source = ReadFile(file);
  ASSERT_NE(true, source.IsEmpty());

  //> Own
  JsHttpRequestProcessor processor(isolate, source);
  
  map<string, string> output;
  EXPECT_EQ(true, processor.Initialize(&options, &output));
  EXPECT_EQ(true, ProcessEntries(&processor, kSampleSize, kSampleRequests));
  PrintMap(&output);
}


TEST(V8, ProcessTopOne) {
  const int kSampleSize = 1;
  StringHttpRequest kSampleRequests[kSampleSize] = {
    StringHttpRequest("/process.cc", "localhost", "google.com", "firefox")
  };
  int argc = 2;
  string path = kPathToTestScripts+string("scripts/count-hosts.js");
  const char* p = path.c_str();
  const char* argv[] = {"", p};

  v8::V8::InitializeICU();
  map<string, string> options;
  string file;
  ParseOptions(argc, argv, options, &file);
  ASSERT_NE(true, file.empty());

  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Handle<String> source = ReadFile(file);
  ASSERT_NE(true, source.IsEmpty());

  //> Own
  JsHttpRequestProcessor processor(isolate, source);
  
  // Похоже передаем объект для заполнения
  map<string, string> output;
  EXPECT_EQ(true, processor.Initialize(&options, &output));
  EXPECT_EQ(true, ProcessEntries(&processor, kSampleSize, kSampleRequests));
  PrintMap(&output);
}

TEST(V8, ProcessOne) {
  int argc = 2;
  string path = kPathToTestScripts+string("scripts/count-hosts.js");
  const char* p = path.c_str();
  const char* argv[] = {"", p};

  v8::V8::InitializeICU();
  map<string, string> options;

  string file;
  ParseOptions(argc, argv, options, &file);
  EXPECT_NE(true, file.empty());

  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Handle<String> source = ReadFile(file);
  ASSERT_NE(true, source.IsEmpty());

  JsHttpRequestProcessor processor(isolate, source);
  
  map<string, string> output;
  EXPECT_EQ(true, processor.Initialize(&options, &output));

  StringHttpRequest request("/", "localhost", "google.net", "firefox");

  EXPECT_EQ(true, processor.Process(&request));
  PrintMap(&output);
}

TEST(V8, ProcessRunFreeFuncFromJS) {
  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a handle scope to hold the temporary references.
  HandleScope handle_scope(isolate);

  // Create a template for the global object where we set the
  // built-in global functions.
  Handle<ObjectTemplate> global = ObjectTemplate::New();
  global->Set(String::New("log"), FunctionTemplate::New(LogCallback));

  // Each processor gets its own context so different processors don't
  // affect each other. Context::New returns a persistent handle which
  // is what we need for the reference to remain after we return from
  // this method. That persistent handle has to be disposed in the
  // destructor.
  v8::Handle<v8::Context> context = Context::New(isolate, NULL, global);

  // Enter the new context so all the following operations take place
  // within it.
  Context::Scope context_scope(context);

  // Можно запускать скрипт
  string file = kPathToTestScripts+string("scripts/free_fuc_call_test.js");

  Handle<String> source = ReadFile(file);
  ASSERT_NE(true, source.IsEmpty());

  TryCatch try_catch;

  // Compile the script and check for errors.
  Handle<Script> compiled_script = Script::Compile(source);
  if (compiled_script.IsEmpty()) {
    String::Utf8Value error(try_catch.Exception());
    ::HttpRequestProcessor::Log(*error);
    // The script failed to compile; bail out.
    return;
  }

  // Run the script!
  Handle<Value> result = compiled_script->Run();
  if (result.IsEmpty()) {
    // The TryCatch above is still in effect and will have caught the error.
    String::Utf8Value error(try_catch.Exception());
    ::HttpRequestProcessor::Log(*error);
    // Running the script failed; bail out.
    return;
  }
}

/*
TEST(V8, CallJSFuncReturnArraySlots) {
  // Return Array<Slot>. Slot - u_int/u_char
  EXPECT_EQ(true, false);
}*/

TEST(V8, ProcessWrapRequest) {
  int argc = 2;
  string path = kPathToTestScripts+string("scripts/count-hosts.js");
  const char* p = path.c_str();
  const char* argv[] = {"", p};

  v8::V8::InitializeICU();
  map<string, string> options;

  string file;
  ParseOptions(argc, argv, options, &file);
  EXPECT_NE(true, file.empty());

  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Handle<String> source = ReadFile(file);
  ASSERT_NE(true, source.IsEmpty());

  JsHttpRequestProcessor processor(isolate, source);

  // Wrap
  // !! Simple in-stack object!

  // FAILED
  //StringHttpRequest request("/", "localhost", "google.net", "firefox");
  //processor.WrapRequest(&request);
}


// http://create.tpsitulsa.com/blog/2009/01/29/v8-objects/
// ! 
TEST(V8, ProcessCreateCppObjectInsideJS) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  //create your function template. Why? After use object?
  Handle<FunctionTemplate> point_template = FunctionTemplate::New();
 
  //get the point's instance template
  Handle<ObjectTemplate> point_instance_template = point_template->InstanceTemplate();
 
  //set its internal field count to one (we'll put references to the C++ point here later)
  point_instance_template->SetInternalFieldCount(1);
 
  //add some properties (x and y)
  point_instance_template->SetAccessor(String::New("x"), &V8Point::GetPointX, &V8Point::SetPointX);
  point_instance_template->SetAccessor(String::New("y"), &V8Point::GetPointY, &V8Point::SetPointY);

  // Look like context not used now!
  // Template Created! 
  // "A template is a blueprint for JavaScript functions and objects in a context"
}

// Conect free function to obj in JS. What be with "this".
TEST(V8, ConnectFreeFunctionToObject) {

}

// new in JS https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/new
TEST(V8, AddFunctionToObject) {

}

TEST(V8, IfFieldIsArray) {

}

/*
TEST(V8, Han) {
	using ::scenarios::V8WrapperImpl;
	using scenarios::dblite;
	dblite* db = new dblite;
	V8WrapperImpl * wrapper = new V8WrapperImpl(db, "..\\scripts\\script.js");

	wrapper->runScript();
}
*/

