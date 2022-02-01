// C++
#include <iostream>
#include <string>

// Other
#include <v8-r16258/include/v8.h>
#include <gtest/gtest.h>

using namespace v8;

TEST(V8, Hello) {
  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);

  // Create a new context.
  Handle<Context> context = Context::New(isolate);

  // Here's how you could create a Persistent handle to the context, if needed.
  Persistent<Context> persistent_context(isolate, context);
  
  // Enter the created context for compiling and
  // running the hello world script. 
  Context::Scope context_scope(context);

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New("'Hello' + ', World!'");

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);
  
  // Run the script to get the result.
  Handle<Value> result = script->Run();
  
  // The persistent handle needs to be eventually disposed.
  persistent_context.Dispose();

  // Convert the result to an ASCII string and print it.
  String::AsciiValue ascii(result);
  printf("%s\n", *ascii);
}

class Point {
 public:
  Point(int x, int y) : x_(x), y_(y) { }
  int x_, y_;
};

TEST(V8, ExposeObject) {
  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);

  // Create a new context.
  Handle<Context> context = Context::New(isolate);

  // Enter the created context for compiling and
  // running the hello world script. 
  Context::Scope context_scope(context);

  //@MayWork?
  Handle<ObjectTemplate> point_templ = ObjectTemplate::New();
  point_templ->SetInternalFieldCount(1);
}
