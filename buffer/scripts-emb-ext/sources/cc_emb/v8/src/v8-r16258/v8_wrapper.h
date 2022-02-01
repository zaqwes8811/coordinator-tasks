#include "virtual_machine.h"
#include "process.h"

// C++
#include <string>

using ::scenarios::dblite;

namespace scenarios {

class V8Wrapper {
 public:  
	 virtual void requestCfg() = 0;  
	 virtual void requestParamst() = 0;

   // !!! Здесь он виртуальный !!! Исправить везде!!
   // Это Грубейшая ошибка!!
   virtual ~V8Wrapper() { }
};

class V8WrapperImpl : public V8Wrapper {
 public:
	explicit V8WrapperImpl(
		::scenarios::dblite* database, 
    std::string JSFileName) 
		  : db_(database), JSFileName_(JSFileName) { }

	virtual void requestCfg();
	virtual void requestParamst();
	void testForExtProcess();
	
	void runScript();

 private:
	::scenarios::dblite* db_;
	std::string JSFileName_;

	// wrap db
	static ::scenarios::dblite* unwrap_dblite(Handle<Object> obj);

	Handle<v8::Value> SomeProperty(Local<v8::String> name, const PropertyCallbackInfo<void>& info);

	static std::string to_string(Local<Value> v);

    //static void Open(const FunctionCallbackInfo<Value>& args);
	v8::Handle<v8::FunctionTemplate> CreateDbLiteTemplate();
	

};
}
