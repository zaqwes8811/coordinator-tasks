
// Other
#include "v8-r16258/include/v8.h"

// App
#include "point_engines.h"

/*
class PointV8EngineImplNoPersistent : public PointV8Engine {
 public:
  static PointV8Engine* CreateForOwn(
      Isolate* isolate, 
      Handle<String> source,
      V8Point* point) 
    {
    PointV8Engine* engine = new PointV8EngineImplNoPersistent(isolate, source, point);
    return engine;
  }
  

 protected:
  PointV8EngineImplNoPersistent(
    Isolate* isolate, 
    Handle<String> source,
    V8Point* point) 
    : isolate_(isolate), source_(source), point_(point) { }

public:
  virtual void RunWithRealPoint(Point* real_point) {
    HandleScope handle_scope(GetIsolate());

    // Create a template for the global object where we set the
    // built-in global functions.
    // Похоже сюда пихаем и нашу точку
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
    //context_.Reset(GetIsolate(), context);

    // Enter the new context so all the following operations take place
    // within it.
    Context::Scope context_scope(context);

    //@Point
    // Install
    Handle<Object> output_obj = WrapPoint(real_point);
    context->Global()->Set(String::New("point_zero"), output_obj);

    //@Point
    // Run Script
    bool success = ExecuteScript(source_, isolate_);
  }

 private:
  Isolate* GetIsolate() { return isolate_; }
  Isolate* isolate_;  // Основным является контекст
  Handle<String> source_;
  
  // "A context is an execution environment that allows separate, 
  // unrelated, JavaScript code to run in a single instance of 
  // V8. You must explicitly specify the context in which you 
  // want any JavaScript code to be run."
  //
  // При экспонировании похоже не нужно.
  Persistent<Context> context_;

  // Может и не нужно будет
  V8Point* point_;

  // Blueprints
  static Persistent<ObjectTemplate> point_template_;


  // Спутано с Persistent - поэтому пока wrap-функция не вынести в V8Point
  // Но вынести ее можно и нужно. Может быть проблема со scope/context
  virtual Handle<Object> WrapPoint(Point* point) {
    HandleScope handle_scope(GetIsolate());
    if (point_template_.IsEmpty()) {
      Handle<ObjectTemplate> raw_template = 
          point_->CreateBlueprint(GetIsolate());

      // Сохраняем, но похоже можно и текущим пользоваться
      point_template_.Reset(GetIsolate(), raw_template);
    }

    // Можно оборачивать реальный объект
    // Сперва нужно сделать пустую обертку
    // Create an empty map wrapper.
    Handle<ObjectTemplate> templ =
        Local<ObjectTemplate>::New(GetIsolate(), point_template_);
    Handle<Object> result = templ->NewInstance();

    // Wrap the raw C++ pointer in an External so it can be referenced
    // from within JavaScript.
    Handle<External> map_ptr = External::New(point);

    // Store the map pointer in the JavaScript wrapper.
    result->SetInternalField(0, map_ptr);
    return handle_scope.Close(result);
  }
};

*/
