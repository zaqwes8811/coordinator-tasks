#ifndef SOME_JS_REQUEST
#define SOME_JS_REQUEST
// cpp
#include <map>
// other
#include <v8-r16258/include/v8.h>

// app
#include "virtual_machine.h"

using namespace std;
using namespace v8;
namespace scenarios {
class JSRequest {
public: 
	//Isolate* isolate, Handle<v8::String> script,
	JSRequest( ::scenarios::dblite* database)
		    : database_(database) { }

	bool Initialize(map<string, string>* opts,
                          map<string, string>* output);

	void wrapDataBase(char * JSFileName);

private:
	//void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args) { }
	bool InstallMaps(map<string, string>* opts, map<string, string>* output);
	Isolate* GetIsolate() { return isolate_; }
	Handle<Object> WrapMap(map<string, string>* obj);
	static Handle<ObjectTemplate> MakeBlueprintMap(Isolate* isolate);
	bool ExecuteScript(Handle<v8::String> script);
	// Callbacks that access maps
	static void MapGet(Local<v8::String> name,
                     const PropertyCallbackInfo<Value>& info);
  static void MapSet(Local<v8::String> name,
                     Local<Value> value,
                     const PropertyCallbackInfo<Value>& info);
	static map<string, string>* UnwrapMap(Handle<Object> obj);
	static void Log(const char* event);
	
	

	Isolate * isolate_;
	Handle<v8::String> script_;
	Persistent<Context> context_;
  Persistent<Function> process_;
	::scenarios::dblite* database_;
	// Lazy initialized?
  //static Persistent<ObjectTemplate> request_template_;
  
	static Persistent<ObjectTemplate> map_template_;
};
string ObjectToString(Local<Value> value);
//void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
}
#endif // SOME_JS_REQUEST
