# coding: utf-8

# std
import re


def write_source(file_name, code):
    f = open(file_name, 'w')
    f.write(('\r\n'.join(code))
    .replace('\r', '@')
    .replace('@', ''))
    f.close()


def remove_cc_comments(string):
    string_to_delete = ""
    regular = re.compile('\/\/.*')
    search_result = regular.search(string)
    if search_result:
        string_to_delete = search_result.group()
    return string.replace(string_to_delete, "")


def delete_double_spaces(string):
    return string.replace("  ", " ")


class Util(object):
    @staticmethod
    def is_array(result, name, var_type, accessor_type):
        # 0 - нет
        # 1 - да
        # 0 - вывод без массивов!
        # 1 - вывод с массивами (по умолчанию 0)
        check_array_print = 0

        if check_array_print == 0:
            if "[" not in name and "vector" not in var_type:
                return result
            else:
                return ""
        if check_array_print == 1:
            if "[" not in name and "vector" not in var_type:
                return result
            else:
                return result

        if accessor_type == "add":
            return "error: bad logic (in make_getter_and_setter_add) or " \
                   + check_array_print + " != 0 or 1, default = 0"
        if accessor_type == "set":
            return "error: bad logic (in make_scalar_setter) or " \
                   + check_array_print + " != 0 or 1, default = 0"
        if accessor_type == "get":
            return "error: bad logic (in make_scalar_getter) or " \
                   + check_array_print + " != 0 or 1, default = 0"

    @staticmethod
    def clear_result(result):
        result = result.replace('\n ', '\n').replace("\n\n\n", "")
        result = result.replace('\n\n', '\n').replace('\n\n}', '\n}')
        result = result.replace("}\n", "}\n\n").replace("}\n\n}", "}\n}").replace("}\n\n\n", "}\n\n")
        return result

