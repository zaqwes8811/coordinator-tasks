#pragma once

#ifndef CC_JS_V8_SCENARIOS
#define CC_JS_V8_SCENARIOS

#include <map>
#include <string>
#include <vector>

// Other
#include <v8-r16258/include/v8.h>

// App
// #include "in_memory_storage/sampler.h" 
//#include "process.h"

typedef unsigned char uchar;

using namespace std;
using namespace v8;

namespace scenarios {

class dblite {
 public:
	bool open(const char* name) { return 0; }
	void close() { }
	bool execute(const char* sql) { return 0; }
	int error_code() { return 3;}
	
	Handle<v8::Value> SomeProperty(Local<v8::String> name, const PropertyCallbackInfo<void>& info);

  static dblite* unwrap_dblite(Handle<Object> obj);
	static std::string to_string(Local<Value> v);
    //static void Open(const FunctionCallbackInfo<Value>& args);

	v8::Handle<v8::FunctionTemplate> CreateDbLiteTemplate();
  
	uchar ctrlModeTgrVar_;
	uchar typeLoadTgrVar_;
	uchar tmitterLockTgrVar___;
	uchar tmitterOnTgrVar___;
	uchar powHalfModeTgrVar_;
	uchar istreamTgrVar_;
	uchar outSynTgrVar_;
	uchar statusTmitterTgrVar_;

  uchar type_tm_digital_;  
  // 1 - norm 0 - not norm  
  uchar ibWork_;        
  // Vtv 1 - lock 0 -unlock  
  uchar exciterLock_;   
  // Power Amplifier // 1 - lock 0 -unlock
  uchar ibPowAmpLock_;     
  
  // Номер работающего возбудителя
  // Vtv 1 = 1 or ETV2 = 2
  uchar idx_oned_etv_;     
   // Vtv 1 = 1 or ETV2 = 2
  uchar idx_oned_etv_q_;    
  // radiation mode 1 -reserve 0 - main
  uchar ibRadioMode_;      
  
  // Transmitter 1 - ready 0 -not ready
  // набор Отказов для вывода на веб-интерфейс?
  uchar transmitterReady_;

};
} // namespace

#endif // CC_JS_V8_SCENARIOS
