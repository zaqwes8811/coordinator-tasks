#include "up2D_v8_auto_gen_r16258.h"
#include "third_party/v8/src/process.h"

using v8::String;
using v8::ObjectTemplate;
using v8::Object;
using v8::HandleScope;
using v8::Handle;
using v8::Local;
using v8::Value;
using v8::External;
using v8::Isolate;
using v8::Number;
using v8::Undefined;
using v8::Context;
using v8::Integer;

namespace app_server_scope {
v8::Handle<v8::ObjectTemplate> RefineInMemoryStorageV8::CreateOwnBlueprint(
      v8::Isolate* isolate) 
  {
  HandleScope handle_scope(isolate);

  Handle<ObjectTemplate> result = ObjectTemplate::New();
  result->SetInternalFieldCount(1);
  result->SetAccessor(
      String::New("voltages"), 
      ZLGetter_voltages);
  result->SetAccessor(
      String::New("command"),
      V8ScalarGetter_command, 
      V8ScalarSetter_command);
  result->SetAccessor(
      String::New("hello"),
      V8ScalarGetter_hello, 
      V8ScalarSetter_hello);

  return handle_scope.Close(result);
}

Handle<Object> RefineInMemoryStorageV8::New(RefineInMemoryStorage* database, v8::Isolate *isolate) {
  HandleScope handle_scope(isolate);
  Context::Scope scope(isolate->GetCurrentContext());

  Handle<ObjectTemplate> raw_template = 
      CreateOwnBlueprint(isolate);    

  Handle<ObjectTemplate> templ =
      Local<ObjectTemplate>::New(isolate, raw_template);

  Handle<Object> result = templ->NewInstance();
  Handle<External> map_ptr = External::New(database);

  result->SetInternalField(0, map_ptr);
  return handle_scope.Close(result);
}

// Vector accessors
void RefineInMemoryStorageV8::ZLGetter_voltages(
      v8::Local<v8::String> name,
      const v8::PropertyCallbackInfo<v8::Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  RefineInMemoryStorage* danger_real_ptr = static_cast<RefineInMemoryStorage*>(ptr);
  Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(
      Isolate::GetCurrent(),
      ArrayMakeBlueprint(
          Isolate::GetCurrent(), 
          LLGetterByIdx_voltages,
          LLSetterByIdx_voltages));
  Handle<Object> instance = templ->NewInstance();
  Handle<External> array_handle = External::New(danger_real_ptr->voltages);
  instance->SetInternalField(0, array_handle);
  info.GetReturnValue().Set<v8::Object>(instance);
}

void RefineInMemoryStorageV8::LLGetterByIdx_voltages(
      uint32_t index, 
      const v8::PropertyCallbackInfo<v8::Value>& info)
  {
  if (index < 9) {
    v8::Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int* array = static_cast<int*>(ptr);
    info.GetReturnValue().Set(Number::New(array[index]));
  } else {
    info.GetReturnValue().Set(Undefined());
  }
}

void RefineInMemoryStorageV8::LLSetterByIdx_voltages(
    uint32_t index,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  if (index < 9) {
    Local<Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int* danger_array = static_cast<int*>(ptr);
    danger_array[index] = value->Int32Value();  
    info.GetReturnValue().Set(v8::Number::New(danger_array[index]));
  } else {
    info.GetReturnValue().Set(Undefined());
  }
}

  // Scalar accessors
void RefineInMemoryStorageV8::V8ScalarGetter_command(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<RefineInMemoryStorage*>(ptr)->command;
  info.GetReturnValue().Set(Integer::New(value));
}

void RefineInMemoryStorageV8::V8ScalarSetter_command(
        v8::Local<v8::String> property, v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  static_cast<RefineInMemoryStorage*>(ptr)->command = value->Int32Value();
}

void RefineInMemoryStorageV8::V8ScalarGetter_hello(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<RefineInMemoryStorage*>(ptr)->hello;
  info.GetReturnValue().Set(Integer::New(value));
}

void RefineInMemoryStorageV8::V8ScalarSetter_hello(
        v8::Local<v8::String> property, v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  static_cast<RefineInMemoryStorage*>(ptr)->hello = value->Int32Value();
}

}