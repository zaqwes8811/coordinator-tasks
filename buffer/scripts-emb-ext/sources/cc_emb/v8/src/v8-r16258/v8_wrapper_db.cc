//#include "config.h"
#include "v8_wrapper.h"

namespace scenarios {

::scenarios::dblite* V8WrapperImpl::unwrap_dblite(Handle<Object> obj) {
	Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	void* ptr = field->Value();
	return static_cast<dblite*>(ptr);
}

Handle<v8::Value> V8WrapperImpl::SomeProperty(Local<v8::String> name, const PropertyCallbackInfo<void>& info) {
// info.Holder() ��������� �� ������ V8 (����� Handle<Object>)
// name ��� ��� ��������, � �������� ����������
	return Integer::New(unwrap_dblite(info.Holder())->error_code());
}

/*
void V8WrapperImpl::Open(const FunctionCallbackInfo<Value>& info) {
  dblite* db = unwrap_dblite(info.This()); // �������� ��������� �� dblite
	std::string sql = to_string(info[0]); // �������� ������ � C++, �������� to_string ��������� � ������ ����� ���� ������
	info.GetReturnValue().Set(v8::BooleanObject::New(db->open(sql.data())));	
}  */

v8::Handle<v8::FunctionTemplate> V8WrapperImpl::CreateDbLiteTemplate() {	

	v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
	v8::Handle<v8::FunctionTemplate> templ = v8::FunctionTemplate::New();
	v8::Handle<v8::ObjectTemplate> instTempl = templ->InstanceTemplate();
	instTempl->SetInternalFieldCount(1);

	v8::Handle<v8::ObjectTemplate> proto = templ->PrototypeTemplate();
    /*proto->SetAccessorProperty(v8::String::NewSymbol("open"), v8::FunctionTemplate::New(

                                   static_cast<v8::FunctionCallback>(&::scenarios::dblite::Open)));*/

// ���������� ��������� ����� ������ �������, ������� ��������� ��� ����� � ���������� HandleScope � �� ���� ��� 
// ������������ ��� ����������� "������" HandleScope - handle_scope
 return handle_scope.Close(templ);
}

std::string V8WrapperImpl::to_string(Local<Value> value) {
	
	v8::String::AsciiValue data(value);
    const char* p = *data;
		if (p == 0) return std::string();
		return std::string(p);
}




}
