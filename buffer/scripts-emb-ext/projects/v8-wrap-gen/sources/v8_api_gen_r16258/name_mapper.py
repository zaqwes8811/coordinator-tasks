# coding: utf-8


class V8Decoders(object):
    @staticmethod
    def cpp_type_to_v8(var_type, accessor_type):
        result = ""
        if accessor_type == "get":
            if var_type == "int" \
                    or var_type == "uint" \
                    or var_type == "char" \
                    or var_type == "uchar":
                result = "Integer"
        if accessor_type == "set":
            if var_type == "int" \
                    or var_type == "uint" \
                    or var_type == "uchar" \
                    or var_type == "char":
                result = "Int32"
        if var_type == "bool":
            result = "Boolean"
        if "string" in var_type:
            result = "v8::String"
        return result

    @staticmethod
    def unroll_unsigned_typedefs(type):
        if type == "uchar":
            return "unsigned char"
        if type == "uint":
            return "unsigned int"
        return type

