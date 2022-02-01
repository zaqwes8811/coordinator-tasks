
// C++
#include <string>
#include <iostream>

// Other
#include <v8-r16258/include/v8.h>
#include <gtest/gtest.h>

// App
#include "process.h"

using namespace v8;

using std::string;
using std::cout;
using std::endl;

using v8::Value;
using v8::PropertyCallbackInfo;

/**
https://code.google.com/p/v8/issues/detail?id=389
*/

const int kArraySize = 32;
static int array[kArraySize];

void ArrayIndexGetter(
    uint32_t index,
    const PropertyCallbackInfo<Value>& info) 
  {
  
  if (index < kArraySize) {
    v8::Local<v8::Object> self = info.Holder();
    info.GetReturnValue().Set(v8::Number::New(array[index]));
  } else {
    info.GetReturnValue().Set(Undefined());
  }
}

void ArrayIndexSetter(
    uint32_t index,
    Local<Value> value,
    const PropertyCallbackInfo<Value>& info) 
  { 
  if (index < kArraySize) {
    v8::Local<v8::Object> self = info.Holder();
    Handle<External> field = Handle<External>::Cast(self->GetInternalField(0));
    void* ptr = field->Value();
    int* array_tmp = static_cast<int*>(ptr);
    
    // Как извлечь значение?
    array_tmp[index] = ObjectToInt(value);

    info.GetReturnValue().Set(v8::Number::New(array_tmp[index]));
  } else {
    info.GetReturnValue().Set(Undefined());
  }
}

TEST(V8, Indexed) {
  array[0] = 10;
  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  HandleScope handle_scope(isolate);

  // YGetter/YSetter are so similar they are omitted for brevity
  Handle<ObjectTemplate> global_templ = ObjectTemplate::New();
  global_templ->Set(String::New("log"), FunctionTemplate::New(LogCallback));	
	
  // Only now create context? Local handle!
  Handle<Context> context = Context::New(isolate, NULL, global_templ);
  Context::Scope context_scope(context);

  ///@Workspace
  // Blueprint
  Handle<ObjectTemplate> blueprint = 
      ArrayMakeBlueprint(isolate, ArrayIndexGetter, ArrayIndexSetter);

  // Wrap
  Handle<Object> wrap = blueprint->NewInstance();

  // Put link
  Handle<External> v8_array = External::New(array);

  // Store the map pointer in the JavaScript wrapper.
  wrap->SetInternalField(0, v8_array);

  context->Global()->Set(String::New("v8_array"), wrap);

  ///@RunScript
  // Create a string containing the JavaScript source code.
  Handle<String> source = String::New("log(v8_array[0]);log(v8_array[32]);v8_array[1] = 11;log(v8_array[1]);");
  

  // Compile the source code.
  Handle<Script> script = Script::Compile(source);
  
  // Run the script to get the result.
  Handle<Value> result = script->Run();

  EXPECT_EQ(11, array[1]);

  // Convert the result to an ASCII string and print it.
  String::AsciiValue ascii(result);
} 



// This function returns a new array with three elements, x, y, and z.
Handle<Array> NewPointArray(int x, int y, int z) {
  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  // Create handle STACK!!
  HandleScope handle_scope(isolate);

  // Create a new context. Локальный!
  Handle<Context> context = Context::New(isolate);
  
  // Enter the created context for compiling and
  // running the hello world script. 
  // Создать нужно обязательно.
  Context::Scope context_scope(context);

  // Create a new empty array.
  // FAIL!!!
  // http://stackoverflow.com/questions/7015068/why-does-creating-new-v8array-before-v8scope-cause-segmentation-fault-but-v?rq=1
  //Handle<Array> array = Array::New(3);
  Local<Array> array = Array::New(3);

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

TEST(V8, ReturnArray) {
  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  // Create handle STACK!!
  HandleScope handle_scope(isolate);

  /// "!! You must explicitly specify the context 
  //    in which you want any JavaScript code to be run."

  // Create a new context. Локальный!
  Handle<Context> context = Context::New(isolate);
  
  /// !!Enter!! the created context for compiling and
  // running the hello world script. 
  // Создать нужно обязательно.
  Context::Scope context_scope(context);

  Handle<Array> array = NewPointArray(0, 1, 2);

  Local<Value> value = array->Get(0);
  int out_value = value->ToObject()->Int32Value ();
  ASSERT_EQ(0, out_value);
}


TEST(V8, ReturnArrayUnroll) {
// Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  // Create handle STACK!!
  HandleScope handle_scope(isolate);

  // Create a new context. Локальный!
  Handle<Context> context = Context::New(isolate);
  
  // Enter the created context for compiling and
  // running the hello world script. 
  // Создать нужно обязательно.
  Context::Scope context_scope(context);

  Local<Array> array = Array::New(3);

  // Return an empty result if there was an error creating the array.
  ASSERT_NE(true, array.IsEmpty());

  array->Set(0, Integer::New(1));
  array->Set(1, Integer::New(2));
  array->Set(2, Integer::New(3));

  Local<Value> value = array->Get(0);
  int out_value = value->ToObject()->Int32Value();
	std::cout << "out value : " <<out_value << endl;
  ASSERT_EQ(1, out_value);
}

TEST(V8, HelloWorld) {
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


int x = 0;
void XGetter(Local<String> property, 
             const PropertyCallbackInfo<Value>& info) {
  info.GetReturnValue().Set(Integer::New(x));
}
    
void XSetter(Local<String> property, Local<Value> value,
              const PropertyCallbackInfo<void>& info) {
  x = value->Int32Value();
}

TEST(V8, GlobalXetter) {
  // Get the default Isolate created at startup.
  Isolate* isolate = Isolate::GetCurrent();

  // Create a handle scope to hold the temporary references.
  HandleScope handle_scope(isolate);

  // YGetter/YSetter are so similar they are omitted for brevity
  Handle<ObjectTemplate> global_templ = ObjectTemplate::New();
  global_templ->SetAccessor(String::New("x"), XGetter, XSetter);
  global_templ->Set(String::New("loge"), FunctionTemplate::New(LogCallback));	
	
  // Only now create context? Local handle!
  Handle<Context> context = Context::New(isolate, NULL, global_templ);
  Context::Scope context_scope(context);

  // Run
  // Можно запускать скрипт
  string file = "./scripts/free_var_xet_test.js";

  Handle<String> source = ReadFile(file);
  EXPECT_NE(true, source.IsEmpty());

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

TEST(V8, CreateObjectOnJSSide) {
/*
v8::Handle<v8::FunctionTemplate> point_templ = 
v8::FunctionTemplate::New();
  point_templ->SetClassName(v8::String::New("IntArray"));
  v8::Handle<v8::ObjectTemplate> point_inst = point_templ-
>InstanceTemplate();
  point_inst->SetIndexedPropertyHandler(GetPointX,SetPointX);
  global->Set(v8::String::New("IntArray"),point_templ);
  global->Set(v8::String::New("IntArray"), 
v8::FunctionTemplate::New(double_array));

  // Create a new execution environment containing the built-in
  // functions
  v8::Handle<v8::Context> context = v8::Context::New(NULL, global);

the get and set functions are

v8::Handle<v8::Value> GetPointX(uint32_t index, const v8::AccessorInfo 
&info) {
	v8::Local<v8::Object> self = info.Holder();
	return  v8::Number::New(10);
  }
v8::Handle<v8::Value> SetPointX(uint32_t index, v8::Local<v8::Value> value,
                 const v8::AccessorInfo& info) {
    v8::Local<v8::Object> self = info.Holder();
	return v8::Undefined();
}

when running compiled exe i can go
var a = new IntArray();
a[0];//return nothing
a[0]=2;
a[0];//returns 2 and I would expect it to return 10
*/
}
