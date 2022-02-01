#include "js_request.h"
#include "process.h"
namespace scenarios {
//Persistent<ObjectTemplate> JSRequest::request_template_;
Persistent<ObjectTemplate> JSRequest::map_template_;
void JSRequest::Log(const char* event) {
  printf("Logged: %s\n", event);
}

map<string, string>* JSRequest::UnwrapMap(Handle<Object> obj) {
  Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
  void* ptr = field->Value();
  return static_cast<map<string, string>*>(ptr);
}

bool JSRequest::ExecuteScript(Handle<String> script) {
  HandleScope handle_scope(GetIsolate());

  // We're just about to compile the script; set up an error handler to
  // catch any exceptions the script might throw.
  TryCatch try_catch;

  // Compile the script and check for errors.
  Handle<Script> compiled_script = Script::Compile(script);
  if (compiled_script.IsEmpty()) {
    String::Utf8Value error(try_catch.Exception());
    Log(*error);
    // The script failed to compile; bail out.
    return false;
  }

  // Run the script!
  Handle<Value> result = compiled_script->Run();
  if (result.IsEmpty()) {
    // The TryCatch above is still in effect and will have caught the error.
    String::Utf8Value error(try_catch.Exception());
    Log(*error);
    // Running the script failed; bail out.
    return false;
  }
  return true;
}

string ObjectToString(Local<Value> value) {
  String::Utf8Value utf8_value(value);
  return string(*utf8_value);
}

void JSRequest::MapGet(Local<String> name,
                       const PropertyCallbackInfo<Value>& info) {
  // Fetch the map wrapped by this object.
  map<string, string>* obj = UnwrapMap(info.Holder());

  // Convert the JavaScript string to a std::string.
  string key = ObjectToString(name);

  // Look up the value if it exists using the standard STL ideom.
  map<string, string>::iterator iter = obj->find(key);

  // If the key is not present return an empty handle as signal
  if (iter == obj->end()) return;

  // Otherwise fetch the value and wrap it in a JavaScript string
  const string& value = (*iter).second;
  info.GetReturnValue().Set(
      String::New(value.c_str(), static_cast<int>(value.length())));
}

void JSRequest::MapSet(Local<String> name,
                                    Local<Value> value_obj,
                                    const PropertyCallbackInfo<Value>& info) {
  // Fetch the map wrapped by this object.
  map<string, string>* obj = UnwrapMap(info.Holder());

  // Convert the key and value to std::strings.
  string key = ObjectToString(name);
  string value = ObjectToString(value_obj);

  // Update the map.
  (*obj)[key] = value;

  // Return the value; any non-empty handle will work.
  info.GetReturnValue().Set(value_obj);
}

Handle<ObjectTemplate> JSRequest::MakeBlueprintMap(
    Isolate* isolate) {
  HandleScope handle_scope(isolate);

  Handle<ObjectTemplate> result = ObjectTemplate::New();
  result->SetInternalFieldCount(1);
  result->SetNamedPropertyHandler(MapGet, MapSet);

  // Again, return the result through the current handle scope.
  return handle_scope.Close(result);
}

Handle<Object> JSRequest::WrapMap(map<string, string>* obj) {
  // Handle scope for temporary handles.
  HandleScope handle_scope(GetIsolate());

  // Fetch the template for creating JavaScript map wrappers.
  // It only has to be created once, which we do on demand.
  if (map_template_.IsEmpty()) {
    // lazy!
    Handle<ObjectTemplate> raw_template = MakeBlueprintMap(GetIsolate());
    map_template_.Reset(GetIsolate(), raw_template);
  }
  Handle<ObjectTemplate> templ =
      Local<ObjectTemplate>::New(GetIsolate(), map_template_);

  // Create an empty map wrapper.
  Handle<Object> result = templ->NewInstance();

  // Wrap the raw C++ pointer in an External so it can be referenced
  // from within JavaScript.
  Handle<External> map_ptr = External::New(obj);

  // Store the map pointer in the JavaScript wrapper.
  result->SetInternalField(0, map_ptr);

  // Return the result through the current handle scope.  Since each
  // of these handles will go away when the handle scope is deleted
  // we need to call Close to let one, the result, escape into the
  // outer handle scope.
  return handle_scope.Close(result);
}

bool JSRequest::InstallMaps(map<string, string>* opts,
                                         map<string, string>* output) {
  HandleScope handle_scope(GetIsolate());

  // Wrap the map object in a JavaScript wrapper
  Handle<Object> opts_obj = WrapMap(opts);

  v8::Local<v8::Context> context =
      v8::Local<v8::Context>::New(GetIsolate(), context_);

  // Set the options object as a property on the global object.
  context->Global()->Set(String::New("options"), opts_obj);

  // ! Нужно получить новую форму для объекта
  Handle<Object> output_obj = WrapMap(output);
  context->Global()->Set(String::New("output"), output_obj);

  return true;
}

bool JSRequest::Initialize(std::map<string,string> *opts, std::map<string,string> *output) {
	// Create a handle scope to hold the temporary references.
	HandleScope handle_scope(GetIsolate());

  // Create a template for the global object where we set the
  // built-in global functions.
  Handle<ObjectTemplate> global = ObjectTemplate::New();

  // Expose logger
  global->Set(String::New("log"), FunctionTemplate::New(LogCallback));

  // Each processor gets its own context so different processors don't
  // affect each other. Context::New returns a persistent handle which
  // is what we need for the reference to remain after we return from
  // this method. That persistent handle has to be disposed in the
  // destructor.
  v8::Handle<v8::Context> context = Context::New(GetIsolate(), NULL, global);
  context_.Reset(GetIsolate(), context);

  // Enter the new context so all the following operations take place
  // within it.
  Context::Scope context_scope(context);

  // Make the options mapping available within the context
  if (!InstallMaps(opts, output))
    return false;

  // Compile and run the script
  if (!ExecuteScript(script_))
    return false;

  // The script compiled and ran correctly.  Now we fetch out the
  // Process function !!from the global!! object.
  Handle<String> process_name = String::New("Process");
  Handle<Value> process_val = context->Global()->Get(process_name);

  // If there is no Process function, or if it is not a function,
  // bail out
  if (!process_val->IsFunction()) return false;

  // It is a function; cast it to a Function
  Handle<Function> process_fun = Handle<Function>::Cast(process_val);

  // Store the function in a Persistent handle, since we also want
  // that to remain after this call returns
  process_.Reset(GetIsolate(), process_fun);

  // All done; all went well
  return true;
return true;
}

void JSRequest::wrapDataBase(char* JSFileName) {
	map<string, string> options;
  map<string, string> output;

  // Expose not as object. Can't get keys.  
  options.insert(pair<string, string>("key", "value"));
  options.insert(pair<string, string>("key1", "value"));
  options.insert(pair<string, string>("key2", "value"));
  options.insert(pair<string, string>("key3", "value"));

	// V8
	int argc = 2;
    const char* argv[] = {"", JSFileName};
  v8::V8::InitializeICU();
  string file;
 
	ParseOptions(argc, argv, options, &file);	
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  Handle<String> source = ReadFile(file);   
	//vamp
	isolate_ = isolate;
	script_ = source;

  Initialize(&options, &output);
}

}
