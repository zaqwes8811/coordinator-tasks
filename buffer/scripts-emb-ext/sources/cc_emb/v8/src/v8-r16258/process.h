
#pragma once

#include <map>
#include <string>

// Other
#include <v8-r16258/include/v8.h>

using namespace std;
using namespace v8;

// These interfaces represent an existing request processing interface.
// The idea is to imagine a real application that uses these interfaces
// and then add scripting capabilities that allow you to interact with
// the objects through JavaScript.

/**
 * A simplified http request. URL?
 */
class HttpRequest {
 public:
  virtual ~HttpRequest() { }
  virtual const string& Path() = 0;
  virtual const string& Referrer() = 0;
  virtual const string& Host() = 0;
  virtual const string& UserAgent() = 0;
};


/**
 * The abstract superclass of http request processors.
 */
class HttpRequestProcessor {
 public:
  virtual ~HttpRequestProcessor() { }

  // Initialize this processor.  The map contains options that control
  // how requests should be processed.
  virtual bool Initialize(map<string, string>* options,
                          map<string, string>* output) = 0;

  // Process a single request.
  virtual bool Process(HttpRequest* req) = 0;

  static void Log(const char* event);
};


/**
 * An http request processor that is scriptable using JavaScript.
 */
class JsHttpRequestProcessor : public HttpRequestProcessor {
 public:
  // Creates a new processor that processes requests by invoking the
  // Process function of the JavaScript script given as an argument.
	 JsHttpRequestProcessor(Isolate* isolate, Handle<v8::String> script)
      : isolate_(isolate), script_(script) { }
  virtual ~JsHttpRequestProcessor();

  virtual bool Initialize(map<string, string>* opts,
                          map<string, string>* output);
  virtual bool Process(HttpRequest* req);

 protected:
  // Execute the script associated with this processor and extract the
  // Process function.  Returns true if this succeeded, otherwise false.
	 bool ExecuteScript(Handle<v8::String> script);

  // Wrap the options and output map in a JavaScript objects and
  // install it in the global namespace as 'options' and 'output'.
  bool InstallMaps(map<string, string>* opts, map<string, string>* output);

  // Constructs the template that describes the JavaScript wrapper
  // type for requests.
  // foreach() для так экспонированных хешей не работают!
  //   но на прямоую обратится можно. Возможно это нормально, т.к. 
  //   JS принимает как экспонированный класс, и возможно это еще не полноценный
  //   JS-объект
  static Handle<ObjectTemplate> MakeRequestTemplate(Isolate* isolate);
  static Handle<ObjectTemplate> MakeBlueprintMap(Isolate* isolate);

  // Callbacks that access the individual fields of request objects.
	static void GetPath(Local<v8::String> name,
                      const PropertyCallbackInfo<Value>& info);
	static void GetReferrer(Local<v8::String> name,
                          const PropertyCallbackInfo<Value>& info);
	static void GetHost(Local<v8::String> name,
                      const PropertyCallbackInfo<Value>& info);
	static void GetUserAgent(Local<v8::String> name,
                           const PropertyCallbackInfo<Value>& info);

  // Callbacks that access maps
	static void MapGet(Local<v8::String> name,
                     const PropertyCallbackInfo<Value>& info);
  static void MapSet(Local<v8::String> name,
                     Local<Value> value,
                     const PropertyCallbackInfo<Value>& info);

  // Utility methods for wrapping C++ objects as JavaScript objects,
  // and going back again.
  Handle<Object> WrapMap(map<string, string>* obj);
  static map<string, string>* UnwrapMap(Handle<Object> obj);

//protected:  // TODO:
public:
  Handle<Object> WrapRequest(HttpRequest* obj);
  
private:
  static HttpRequest* UnwrapRequest(Handle<Object> obj);

  Isolate* GetIsolate() { return isolate_; }

  Isolate* isolate_;
  Handle<v8::String> script_;
  Persistent<Context> context_;
  Persistent<Function> process_;
  
  // Lazy initialized?
  static Persistent<ObjectTemplate> request_template_;

  static Persistent<ObjectTemplate> map_template_;
};


/**
 * A simplified http request.
 */
class StringHttpRequest : public HttpRequest {
 public:
  StringHttpRequest(const string& path,
                    const string& referrer,
                    const string& host,
                    const string& user_agent);
  virtual const string& Path() { return path_; }
  virtual const string& Referrer() { return referrer_; }
  virtual const string& Host() { return host_; }
  virtual const string& UserAgent() { return user_agent_; }
 private:
  string path_;
  string referrer_;
  string host_;
  string user_agent_;
};


// Reads a file into a v8 string.
Handle<v8::String> ReadFile(const string& name);
void PrintMap(map<string, string>* m);
bool ProcessEntries(HttpRequestProcessor* processor, int count,
                    StringHttpRequest* reqs);
void ParseOptions(int argc,
                  const char* argv[],
                  map<string, string>& options,
                  string* file);

void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

// Convert a JavaScript string to a std::string.  To not bother too
// much with string encodings we just use ascii.
string ObjectToString(Local<Value> value);
int ObjectToInt(Local<Value> value);

bool ExecuteScript(Handle<String> script, Isolate* isolate);

v8::Handle<v8::ObjectTemplate> ArrayMakeBlueprint(
    Isolate* isolate,
    IndexedPropertyGetterCallback getter,
    IndexedPropertySetterCallback setter=0);

