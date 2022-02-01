//#include "config.h"

// cpp
#include <map>
#include <string>

// app
#include "v8_wrapper.h"
#include "js_request.h"

using std::map;
using std::string;
using ::scenarios::JSRequest;

namespace scenarios {

void V8WrapperImpl::requestCfg() { }

void V8WrapperImpl::requestParamst() { }

void V8WrapperImpl::runScript() {
	//V8WrapperImpl::v8_set_accessor_t_idx_oned_etv_();
}

void V8WrapperImpl::testForExtProcess() {
	

}


}  // end namespace













/*

static void v8_get_t_idx_oned_etv_(Local<String> name,
																	 const PropertyCallbackInfo<Value>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
	int value = static_cast<::scenarios::dblite*>(ptr)->idx_oned_etv_;

  // New api
  info.GetReturnValue().Set(Integer::New(value));
}

static void v8_set_t_idx_oned_etv_(Local<String> property, Local<Value> value,
																	 const PropertyCallbackInfo<void>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
	static_cast<::scenarios::dblite*>(ptr)->idx_oned_etv_ = value->Int32Value();
}

void v8_set_accessor_t_idx_oned_etv_() {
	Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  //create your function template. Why? After use object?
  Handle<FunctionTemplate> db_template = FunctionTemplate::New();
 
  //get the point's instance template
  Handle<ObjectTemplate> db_instance_template = db_template->InstanceTemplate();
 
  //set its internal field count to one (we'll put references to the C++ point here later)
  db_instance_template->SetInternalFieldCount(1);
 
  //add some properties (x and y)
  db_instance_template->SetAccessor(String::New("idx_oned_etv_"), 
		static_cast<v8::AccessorGetterCallback>(&scenarios::V8WrapperImpl::v8_get_t_idx_oned_etv_),
		static_cast<v8::AccessorSetterCallback>(&scenarios::V8WrapperImpl::v8_set_t_idx_oned_etv_));

	//::scenarios::dblite* db =;
//  Local<Object> databaseObj = db_instance_template->NewInstance();
 // databaseObj->SetInternalField(0, External::New(db_));
	
	db_instance_template->Set(String::New("log"), FunctionTemplate::New(LogCallback));	
	//db_instance_template->SetAccessorProperty(v8::String::NewSymbol("RequestParamsTempl"),
		//v8::FunctionTemplate::New(
		//static_cast<v8::FunctionCallback>(&::scenarios::V8WrapperImpl::requestParamsTempl)));

	Handle<Context> context = Context::New(isolate, NULL, db_instance_template);
  Context::Scope context_scope(context);

	// Run
  // Можно запускать скрипт
 // string file = "..\\scripts\\script.js";
	string file = JSFileName_;

  Handle<String> source = ReadFile(file);
 // EXPECT_NE(true, source.IsEmpty());

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
}*/