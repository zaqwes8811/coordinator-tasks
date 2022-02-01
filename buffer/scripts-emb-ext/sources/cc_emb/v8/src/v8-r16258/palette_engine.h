#pragma once

// App
#include "process.h"
#include "palette_v8.h"

class PaletteEngine {
 public:
  PaletteEngine(
      Isolate* isolate,
      Handle<String> source,
      raw_objects::Palette* palette)
        : isolate_(isolate), source_(source) {
    HandleScope handle_scope(GetIsolate());

    // Create a template for the global object where we set the
    // built-in global functions.
    Handle<ObjectTemplate> global = ObjectTemplate::New();

    // Expose logger
    global->Set(String::New("log"), 
        FunctionTemplate::New(LogCallback));

    //@Point:
    // Create context?
    // Each processor gets its own context so different processors don't
    // affect each other. Context::New returns a persistent handle which
    // is what we need for the reference to remain after we return from
    // this method. That persistent handle has to be disposed in the
    // destructor.
    v8::Handle<v8::Context> context = 
        Context::New(GetIsolate(), NULL, global);
    context_.Reset(GetIsolate(), context);

    // Enter the new context so all the following operations take place
    // within it.
    Context::Scope context_scope(GetIsolate(), context_);

    V8Palette v8_palette(isolate);

    //@Point
    // Install
    Handle<Object> output_obj = v8_palette.Forge(palette);

    context->Global()->Set(String::New("palette"), output_obj);

    //@Point
    // Run Script
    bool success = ExecuteScript(source_, isolate_);
  }

 private:
  Isolate* GetIsolate() { return isolate_; }
  Isolate* isolate_;  // Основным является контекст
  Handle<String> source_;
  Persistent<Context> context_;
};
