
// C++
#include <string>

// Third party
#include <gtest/gtest.h>
#include <v8-r10448/include/v8.h>

// App
#include <raw/point.h>

using std::string;

using namespace v8;

using raw_objects::Point;

// This function returns a new array with three elements, x, y, and z.

Handle<Array> NewPointArray(int x, int y, int z) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();

  // We will be creating temporary handles so we use a handle scope.
  HandleScope handle_scope;//();//isolate);

  // Create a new empty array.
  Handle<Array> array = Array::New(3);

  // Return an empty result if there was an error creating the array.
  if (array.IsEmpty())
    return Handle<Array>();

  // Fill out the values
  array->Set(0, Integer::New(x));
  array->Set(1, Integer::New(y));
  array->Set(2, Integer::New(z));

  // Return the value through Close.
  return handle_scope.Close(array);
}

int x, y;

Handle<Value> XGetter(Local<String> property,
                        const AccessorInfo& info) {
    return Integer::New(x);
  }

void XSetter(Local<String> property, Local<Value> value,
           const AccessorInfo& info) {
    x = value->Int32Value();
}

TEST(V810448, Simple) {
// Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  HandleScope handle_scope;//(isolate);

  // Create a new context.
  Handle<ObjectTemplate> global = ObjectTemplate::New();
  Persistent<Context>  context = Context::New(NULL, global);

  // Enter the context for compiling and running the hello world script.
  Context::Scope context_scope(context);

  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New("'Hello' + ', World!'");

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);

  // Run the script to get the result.
  Handle<Value> result = script->Run();

  // Convert the result to an UTF8 string and print it.
  String::Utf8Value utf8(result);
  printf("%s\n", *utf8);

  context.Dispose();
}

TEST(V810448, GlobalVars) {
}
