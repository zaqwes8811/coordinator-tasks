//#include "config.h"
#include "process.h"

class JsHttpRequestProcessorTestExt : 
    public JsHttpRequestProcessor 
  {
  public:
    JsHttpRequestProcessorTestExt(Isolate* isolate, Handle<v8::String> script) : 
      JsHttpRequestProcessor(isolate, script) { }

      bool InstallMapsTest(map<string, string>* opts, map<string, string>* output) {
        this->InstallMaps(opts, output);
      }
};