# coding: utf-8


class MakerV8ScalarFieldAccessor(object):
    """
    About:
        Работает с одной переменной.
    """

    def __init__(self, class_name=None, variable_node=None):
        self.class_name_ = class_name
        self.variable_node_ = variable_node

        # Регистрируем типы
        self.V8_GETTER_RECODER_ = {'int': 'Integer',
                                   'uint': 'Integer',
                                   'uchar': 'Integer',
                                   'bool': 'Boolean'}
        self.V8_SETTER_RECODER_ = {'int': 'Int32',
                                   'uint': 'Int32',
                                   'uchar': 'Int32',
                                   'bool': 'Boolean'}

    def is_array(self):
        """
        About:
            vector vec;
            int array[]
        """
        result = False
        if '[' in self.variable_node_.name or 'vector' in self.variable_node_.type.name:
            result = True

        return result

    def get_array_size(self):
        if self.is_array():
            return 0
        else:
            return None

    def __str__(self):
        return str(self.class_name_)


    def get_wrapper_class_name(self):
        return 'ForgeV8' + self.class_name_ + 's'

    def make_getter(self):
        """
        About:
        class Point {
            public:  // bad, but now be it
            int x;
        };

        Notes:
            static method is BAD!
        """
        field_type, field_name, class_name = (self.variable_node_.type.name,
                                              self.variable_node_.name,
                                              self.class_name_)

        if field_type not in self.V8_GETTER_RECODER_:
            return "// Map not found: " + field_type + ' - ' + field_name, None

        def make_getter_header(field_name_local):
            return 'V8ScalarGetter_' + field_name_local + '(\r\n' + \
                   '    v8::Local<v8::String> name,\r\n' + \
                   '    const v8::PropertyCallbackInfo<v8::Value>& info)'

        template = make_getter_header(field_name) + ' \r\n' + \
                   '  {\r\n' + \
                   '  Local<Object> self = info.Holder();\r\n' + \
                   '  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));\r\n' + \
                   '  void* ptr = wrap->Value();\r\n' + \
                   '  ' + field_type + ' value = static_cast<' + class_name + '*>(ptr)->' + field_name + ';\r\n' + \
                   '  info.GetReturnValue().Set(' + self.V8_GETTER_RECODER_[field_type] + '::New(value));\r\n' + \
                   '}'

        return template, make_getter_header(field_name) + ';\r\n'

    # BUGS!! Нужно тоже пропустить через регистратор!!
    @property
    def setter_name(self):
        return 'V8ScalarSetter_' + self.variable_node_.name

    def getter_name(self):
        return 'V8ScalarGetter_' + self.variable_node_.name

    def make_scalar_setter(self):
        def make_setter_header(field_name_local):
            return 'V8ScalarSetter_' + field_name_local + '(\r\n' + \
                   '        v8::Local<v8::String> property, v8::Local<v8::Value> value,\r\n' + \
                   '        const v8::PropertyCallbackInfo<void>& info)'

        field_type, field_name, class_name = (self.variable_node_.type.name,
                                              self.variable_node_.name,
                                              self.class_name_)

        if field_type not in self.V8_GETTER_RECODER_:
            return "Map not found", None

        template = make_setter_header(field_name) + ' \r\n' + \
                   '  {\r\n' + \
                   '  Local<Object> self = info.Holder();\r\n' + \
                   '  Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));\r\n' + \
                   '  void* ptr = wrap->Value();\r\n' + \
                   '  static_cast<' + class_name + '*>(ptr)->' + field_name + ' = value->' + self.V8_SETTER_RECODER_[
                       field_type] + 'Value();\r\n' + \
                   '}'

        return template, make_setter_header(field_name)

    def wrap_scalar_getters_header(self, declarations):
        code_result = []
        for decl in declarations:
            code_result.append('  static void ' + decl[0])
        return code_result


    def make_v8_class_name(self, name):
        return name + 'V8'

    def do_scalar_connecters(self, dec_wrappers):
        # zaqwes
        impls = []
        for elem in dec_wrappers:
            if not elem.is_array():
                g, s, n = elem.getter_name(), elem.setter_name, elem.variable_node_.name
                impls.append('  result->SetAccessor(\n      String::New("' + n + '"),\n      ' + g + ', \n      ' + s)

        return impls

    def make_getters_decl(self, elem):
        if not elem.is_array():
            i, d = elem.make_getter()
            if d:
                return '  static void ' + d# TODO: wtf + ';\n'
            else:
                print d
                return None

    def make_setter_decl(self, elem):
        if not elem.is_array():
            i, d = elem.make_scalar_setter()
            if d:
                return '  static void '+ d + ';\n'
            else:
                return None
                print d

    def make_setter_impl(self, elem, class_name):
        if not elem.is_array():
            impl, d = elem.make_scalar_setter()
            if d:
                return 'void ' + self.make_v8_class_name(class_name) + '::' + impl + '\n'
            else:
                print impl
                return None

    def make_getter_impl(self, elem, class_name):
        if not elem.is_array():
            impl, d = elem.make_getter()
            if d:
                return 'void ' + self.make_v8_class_name(class_name) + '::' + impl + '\n'
            else:
                print impl
                return None
