#include "palette_v8.h"

// App
#include <v8-r16258/process.h>

v8::Persistent<v8::ObjectTemplate> V8Palette::point_array_blueprint_;
v8::Persistent<v8::ObjectTemplate> V8Palette::int_array_blueprint_;
Persistent<ObjectTemplate> V8Palette::own_blueprint_;

using raw_objects::Point;
using raw_objects::Palette;

V8Palette::V8Palette(Isolate* isolate) : isolate_(isolate) { 
  // Нужно создать Persistent!
  HandleScope handle_scope(isolate);
  Context::Scope scope(isolate->GetCurrentContext());

  // Вот их похоже нужно создавать здесь
  // Шаблон массива
  if (int_array_blueprint_.IsEmpty()) {
    Handle<ObjectTemplate> raw_template = 
        ArrayMakeBlueprint(isolate, ArrayIndexGetter, ArrayIndexSetter);

    // Сохраняем, но похоже можно и текущим пользоваться
    int_array_blueprint_.Reset(isolate, raw_template);
  }

  // Шаблон массива
  if (point_array_blueprint_.IsEmpty()) {
    Handle<ObjectTemplate> raw_template = 
        ArrayMakeBlueprint(isolate, PointArrayIndexGetter, PointArrayIndexSetter);

    // Сохраняем, но похоже можно и текущим пользоваться
    point_array_blueprint_.Reset(isolate, raw_template);
  }
}

v8::Handle<v8::ObjectTemplate> V8Palette::MakeBlueprint() {
  HandleScope handle_scope(isolate_);
  Context::Scope scope(isolate_->GetCurrentContext());

  Handle<ObjectTemplate> result = ObjectTemplate::New();
  result->SetInternalFieldCount(1);

  // Connect getter/setter
  result->SetAccessor(String::New("point"), GetPointValue);
  result->SetAccessor(String::New("int_array"), GetIntArrayValue);
  result->SetAccessor(String::New("points_array"), GetPointsArrayValue);

  return handle_scope.Close(result);
}

Handle<Object> V8Palette::Forge(Palette* palette) {
  HandleScope handle_scope(isolate_);
  Context::Scope scope(isolate_->GetCurrentContext());

  if (own_blueprint_.IsEmpty()) {
    Handle<ObjectTemplate> raw_template = 
        this->MakeBlueprint();

    // Сохраняем, но похоже можно и текущим пользоваться
    own_blueprint_.Reset(isolate_, raw_template);
  }

  Handle<ObjectTemplate> templ =
      Local<ObjectTemplate>::New(isolate_, own_blueprint_);

  Handle<Object> result = templ->NewInstance();
  Handle<External> map_ptr = External::New(palette);
  result->SetInternalField(0, map_ptr);

  return handle_scope.Close(result);
}

void V8Palette::ArrayIndexGetter(
    uint32_t index,
    const PropertyCallbackInfo<Value>& info) 
  {
  
  if (index < Palette::MAX_SIZE) {
    v8::Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();
    int* palette = static_cast<int*>(ptr);
    info.GetReturnValue().Set(v8::Number::New(palette[index]));
  } else {
    info.GetReturnValue().Set(Undefined());
  }
}

//@HardObjects
void V8Palette::GetPointValue(
    Local<String> name,
    const PropertyCallbackInfo<Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();

  // Возвращает точку!
  Palette* palette = static_cast<Palette*>(ptr);
  Point* point = &(palette->point_);
  V8Point v8_point(Isolate::GetCurrent());
  info.GetReturnValue().Set<v8::Object>(v8_point.Forge(point, Context::GetCurrent()));
}

void V8Palette::GetIntArrayValue(
      Local<String> name,
      const PropertyCallbackInfo<Value>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();

  // Возвращает точку!
  Palette* palette = static_cast<Palette*>(ptr);

  // Нужно обернуть и вернуть
  // Нужно плучить V8Palette или сделать новый хэндлер
  Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(
      Isolate::GetCurrent(), 
      int_array_blueprint_);

  Handle<Object> instance = templ->NewInstance();
  Handle<External> point_handle = External::New(palette->array_);
  instance->SetInternalField(0, point_handle);
  info.GetReturnValue().Set<v8::Object>(instance);
}

void V8Palette::GetPointsArrayValue(
      Local<String> name,
      const PropertyCallbackInfo<Value>& info) 
  {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();

  // Возвращает точку!
  Palette* palette = static_cast<Palette*>(ptr);

  Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(
      Isolate::GetCurrent(), 
      point_array_blueprint_);

  Handle<Object> instance = templ->NewInstance();
  Handle<External> point_handle = External::New(palette->point_array);
  instance->SetInternalField(0, point_handle);
  info.GetReturnValue().Set<v8::Object>(instance);
}
//@HardObjects

void V8Palette::PointArrayIndexGetter(
    uint32_t index,
    const PropertyCallbackInfo<Value>& info) 
  {
  if (index < Palette::MAX_SIZE) {
    v8::Local<v8::Object> self = info.Holder();
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    void* ptr = wrap->Value();

    // FAILURE!! не то что нужно!
    // На сколько ж стремный язык...
    //Palette* palette = static_cast<Palette*>(ptr);
    // Массив!! Это массив!! Очень стремно и очень важно!
    Point* point = static_cast<Point*>(ptr);
    //Point* point = &palette->point_array[index];

    V8Point v8_point(Isolate::GetCurrent());
    info.GetReturnValue().Set<v8::Object>(v8_point.Forge(&point[index], Context::GetCurrent()));
  } else {
    info.GetReturnValue().Set(Undefined());
  }
}
