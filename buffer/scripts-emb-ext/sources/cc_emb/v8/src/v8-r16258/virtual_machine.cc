//#include "config.h"
#include "virtual_machine.h"

namespace scenarios {

dblite* dblite::unwrap_dblite(Handle<Object> obj) {
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<dblite*>(ptr);
}

/*
Handle<v8::Value> dblite::SomeProperty(Local<v8::String> name, const const PropertyCallbackInfo<void>& info) {
// info.Holder() указывает на объект V8 (класс Handle<Object>)
// name это имя свойства, к которому обращаются
	return Integer::New(unwrap_dblite(info.Holder())->error_code());
}*/

/*
void dblite::Open(const FunctionCallbackInfo<Value>& info) {
  dblite* db = unwrap_dblite(info.This()); // забираем указатель на dblite
	std::string sql = to_string(info[0]); // получаем строку в C++, описание to_string находится в первой части этой статьи
	info.GetReturnValue().Set(v8::BooleanObject::New(db->open(sql.data())));	
}  */

v8::Handle<v8::FunctionTemplate> dblite::CreateDbLiteTemplate() {	

	v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
	v8::Handle<v8::FunctionTemplate> templ = v8::FunctionTemplate::New();
	v8::Handle<v8::ObjectTemplate> instTempl = templ->InstanceTemplate();
	instTempl->SetInternalFieldCount(1);

	v8::Handle<v8::ObjectTemplate> proto = templ->PrototypeTemplate();
    /*proto->SetAccessorProperty(v8::String::NewSymbol("open"), v8::FunctionTemplate::New(
            static_cast<v8::FunctionCallback>(&::scenarios::dblite::Open)));*/

// возвращаем временный хэндл хитрым образом, который переносит наш хэндл в предыдущий HandleScope и не дает ему 
// уничтожиться при уничтожении "нашего" HandleScope - handle_scope
 return handle_scope.Close(templ);
}

std::string dblite::to_string(Local<Value> value) {
	
	v8::String::AsciiValue data(value);
    const char* p = *data;
		if (p == 0) return std::string();
		return std::string(p);
}

void v8_get_idx_oned_etv_(Local<String> name,
               const PropertyCallbackInfo<Value>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<dblite*>(ptr)->idx_oned_etv_;

  // New api
  // return Integer::New(value);
  info.GetReturnValue().Set(Integer::New(value));
}

void v8_set_idx_oned_etv_(Local<String> property, Local<Value> value,
               const PropertyCallbackInfo<void>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  static_cast<dblite*>(ptr)->idx_oned_etv_ = value->Int32Value();
}

void v8_set_accessor_idx_oned_etv_() {
	Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  //create your function template. Why? After use object?
  Handle<FunctionTemplate> point_template = FunctionTemplate::New();
 
  //get the point's instance template
  Handle<ObjectTemplate> point_instance_template = point_template->InstanceTemplate();
 
  //set its internal field count to one (we'll put references to the C++ point here later)
  point_instance_template->SetInternalFieldCount(1);
 
  //add some properties (x and y)
  point_instance_template->SetAccessor(String::New("idx_oned_etv_"), 
												v8_get_idx_oned_etv_, v8_set_idx_oned_etv_);
}


} // namespace scenarios


/*
void GetPointY(Local<String> name,
               const PropertyCallbackInfo<Value>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  int value = static_cast<Point*>(ptr)->y_;

  // New api
  // return Integer::New(value);
  info.GetReturnValue().Set(Integer::New(value));
}

void SetPointY(Local<String> property, Local<Value> value,
               const PropertyCallbackInfo<void>& info) {
  Local<Object> self = info.Holder();
  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  static_cast<Point*>(ptr)->y_ = value->Int32Value();
}*/

/*
void ScriptsV8Impl::runScript(std::string file) {
	
	Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

	Handle<Context> context = Context::New(isolate);
	Context::Scope context_scope(context);

  //Handle<v8::String> source = ReadFile(file);

	Local<Integer> value = Integer::New(1);
	int out_value = value->ToObject()->Int32Value();
	cout << "value : " << out_value << endl;

}*/
