#ifndef ___TESTS_DATA_UP2D_V8_AUTO_GEN_R16258_H_
#define ___TESTS_DATA_UP2D_V8_AUTO_GEN_R16258_H_

// Other
#include <v8.h>

#include "up2D.h"

namespace app_server_scope {
class RefineInMemoryStorageV8 {
 public:
  static v8::Handle<v8::ObjectTemplate> CreateOwnBlueprint(
      v8::Isolate* isolate);

  static v8::Handle<v8::Object> New(RefineInMemoryStorage* database, v8::Isolate *isolate);

  // Vector accessors
  static void ZLGetter_voltages(
      v8::Local<v8::String> name,
      const v8::PropertyCallbackInfo<v8::Value>& info);

  static void LLSetterByIdx_voltages(
    uint32_t index,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void LLGetterByIdx_voltages(
      uint32_t index, 
      const v8::PropertyCallbackInfo<v8::Value>& info);

  // Scalar accessors
  static void V8ScalarGetter_command(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void V8ScalarSetter_command(
        v8::Local<v8::String> property, v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

  static void V8ScalarGetter_hello(
    v8::Local<v8::String> name,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void V8ScalarSetter_hello(
        v8::Local<v8::String> property, v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

};
}
#endif  // ___TESTS_DATA_UP2D_V8_AUTO_GEN_R16258_H_