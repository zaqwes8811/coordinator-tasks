# coding: utf-8
__author__ = 'Igor'

# std
import re

# App
import inner_reuse_local

# Последняя точка
LAST_LEVEL_GETTER_ = "LLGetterByIdx_"
LAST_LEVEL_SETTER_ = "LLSetterByIdx_"

# По обращению к массиву без []
ZERO_LEVEL_GETTER_ = "ZLGetter_"


def is_array(method_to_decorate):
    def is_array_name(var_name_local):
        return "[" in var_name_local

    def wrapper(self):
        if is_array_name(self.var_name_):
            return method_to_decorate(self)
        else:
            return None

    return wrapper


class MakerV8VectorFieldAccessor(object):
    """
    Constraints:
        Работает только с int array[]

        Не рабоатет с:
            any array[][];
            any array[];
    """

    def get_v8_class_name(self):
        return self.class_name_ + 'V8'

    def __init__(self, var_type, var_name, class_name):
        self.var_type_ = var_type
        self.var_name_ = var_name
        self.util_ = inner_reuse_local.Util()
        self.class_name_ = class_name
        self.V8_GETTER_RECODER_ = {'int': 'Integer', 'std::string': 'String', 'bool': 'Boolean'}

    @is_array
    def make_last_level_getter_decl(self):
        return LAST_LEVEL_GETTER_ + self.get_vector_name(self.var_name_) + \
               '(\n' \
               '      uint32_t index, \n' \
               '      const v8::PropertyCallbackInfo<v8::Value>& info)'

    @is_array
    def make_zero_level_getter_decl(self):
        return ZERO_LEVEL_GETTER_ \
               + self.get_vector_name(self.var_name_) + '(\n' + \
               '      v8::Local<v8::String> name,\n' + \
               '      const v8::PropertyCallbackInfo<v8::Value>& info)'

    @is_array
    def make_last_level_getter_impl(self):
        return 'void ' + self.get_v8_class_name() + '::' \
               + self.make_last_level_getter_decl() \
               + '\n  {\n' + \
               '  if (index < ' + self.get_idx_threshold(self.var_name_) + ') {\n' + \
               '    v8::Local<v8::Object> self = info.Holder();\n' + \
               '    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));\n' + \
               '    void* ptr = wrap->Value();\n' + \
               '    ' + self.var_type_ + '* array = static_cast<' + self.var_type_ + '*>(ptr);\n' + \
               '    info.GetReturnValue().Set(' + 'Number' + '::New(array[index])' + ');\n' + \
               '  } else {\n' + \
               '    info.GetReturnValue().Set(Undefined());\n' + \
               '  }\n' + \
               '}\n'

    @is_array
    def make_last_level_setter_decl(self):
        return LAST_LEVEL_SETTER_ \
               + self.get_vector_name(self.var_name_) + '(\n' + \
               '    uint32_t index,\n' + \
               '    v8::Local<v8::Value> value,\n' + \
               '    const v8::PropertyCallbackInfo<v8::Value>& info)'

    @is_array
    def make_last_level_setter_impl(self):
        # .lower()
        return 'void ' + self.get_v8_class_name() + '::' + self.make_last_level_setter_decl() + ' {\n' + \
               '  if (index < ' + self.get_idx_threshold(self.var_name_) + ') {\n' + \
               '    Local<Object> self = info.Holder();\n' + \
               '    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));\n' + \
               '    void* ptr = wrap->Value();\n' + \
               '    ' + self.var_type_ + '* danger_array = static_cast<' + self.var_type_ + '*>(ptr);\n' + \
               '    danger_array[index] = value->' + 'Int32Value' + '();  \n' + \
               '    info.GetReturnValue().Set(v8::' + 'Number' + '::New(danger_array[index]));\n' + \
               '  } else {\n' + \
               '    info.GetReturnValue().Set(Undefined());\n' + \
               '  }\n' + \
               '}\n'

    @is_array
    def make_zero_level_getter_impl(self):
        return 'void ' + self.get_v8_class_name() + '::' + self.make_zero_level_getter_decl() + ' \n  {\n' + \
               '  Local<Object> self = info.Holder();\n' + \
               '  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));\n' + \
               '  void* ptr = wrap->Value();\n' + \
               '  ' + self.class_name_ + '* danger_real_ptr = static_cast<' + self.class_name_ + '*>(ptr);\n' + \
               '  Handle<ObjectTemplate> templ = Local<ObjectTemplate>::New(\n' + \
               '      Isolate::GetCurrent(),\n' + \
               '      ArrayMakeBlueprint(\n' + \
               '          Isolate::GetCurrent(), \n' + \
               '          ' + LAST_LEVEL_GETTER_ + self.get_vector_name(self.var_name_) + \
               ',\n' + \
               '          ' + LAST_LEVEL_SETTER_ + self.get_vector_name(self.var_name_) + \
               "));\n" + \
               '  Handle<Object> instance = templ->NewInstance();\n' + \
               '  Handle<External> array_handle = External::New(danger_real_ptr->' \
               + self.get_vector_name(self.var_name_) + ');\n' + \
               '  instance->SetInternalField(0, array_handle);\n' + \
               '  info.GetReturnValue().Set<v8::Object>(instance);\n' + \
               '}\n'

    @is_array
    def connect_getters_and_setters(self):
        # Затираем, если что-то было по скалярам
        return '  result->SetAccessor(\n      String::New(\"' + \
               self.get_vector_name(self.var_name_) + "\"), \n      " + ZERO_LEVEL_GETTER_ + \
               self.get_vector_name(self.var_name_) + ');'

    @staticmethod
    def get_vector_name(var_name):
        result = var_name
        regular = re.compile('\[.*')
        search_result = regular.search(result)
        if search_result:
            result = result.replace(search_result.group(), "")
        return result

    @staticmethod
    def get_idx_threshold(var_name):
        result = var_name
        index = ""
        regular = re.compile('\[.*')
        search_result = regular.search(result)
        if search_result:
            index = search_result.group()
        index = index.replace("[", "").replace("]", "")
        return index


