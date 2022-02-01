#include "point_v8.h"

// App
#include "raw/point.h"

using v8::Value;
using v8::Handle;
using v8::Local;
using v8::String;
using v8::External;
using v8::Object;
using v8::Integer;
using v8::PropertyCallbackInfo;
using v8::HandleScope;
using v8::Isolate;
using v8::Context;

using v8::ObjectTemplate;

using raw_objects::Point;

v8::Persistent<v8::ObjectTemplate> V8Point::own_blueprint_;

void V8Point::GetPointX(Local<String> name,
                        const PropertyCallbackInfo<Value>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<Point*>(ptr)->x_;
  info.GetReturnValue().Set(Integer::New(value));
}

void V8Point::SetPointX(Local<String> property, Local<Value> value,
                        const PropertyCallbackInfo<void>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  Point* point = static_cast<Point*>(ptr);
  point->x_ = value->Int32Value();
}

void V8Point::GetPointY(Local<String> name, 
                        const PropertyCallbackInfo<Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<Point*>(ptr)->y_;
  info.GetReturnValue().Set(Integer::New(value));
}

void V8Point::SetPointY(Local<String> property, Local<Value> value, 
                        const PropertyCallbackInfo<void>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  static_cast<Point*>(ptr)->y_ = value->Int32Value();
}

v8::Handle<v8::ObjectTemplate> V8Point::MakeBlueprint() {
  HandleScope handle_scope(isolate_);
  Context::Scope scope(isolate_->GetCurrentContext());

  Handle<ObjectTemplate> result = ObjectTemplate::New();
  result->SetInternalFieldCount(1);

  // Connect getter/setter
  result->SetAccessor(String::New("x"), GetPointX, SetPointX);
  result->SetAccessor(String::New("y"), GetPointY, SetPointY);

  return handle_scope.Close(result);
}

v8::Handle<v8::Object> V8Point::Forge(Point* point, v8::Handle<v8::Context> context) {
  HandleScope handle_scope(isolate_);
  Context::Scope scope(isolate_->GetCurrentContext());

  if (own_blueprint_.IsEmpty()) {
    Handle<ObjectTemplate> raw_template = 
        this->MakeBlueprint();

    // Сохраняем, но похоже можно и текущим пользоваться
    own_blueprint_.Reset(isolate_, raw_template);
  }

  // Можно оборачивать реальный объект
  // Сперва нужно сделать пустую обертку
  // Create an empty map wrapper.
  Handle<ObjectTemplate> templ =
      Local<ObjectTemplate>::New(isolate_, own_blueprint_);
  Handle<Object> result = templ->NewInstance();

  // Wrap the raw C++ pointer in an External so it can be referenced
  // from within JavaScript.
  Handle<External> map_ptr = External::New(point);

  // Store the map pointer in the JavaScript wrapper.
  result->SetInternalField(0, map_ptr);
  return handle_scope.Close(result);
}
