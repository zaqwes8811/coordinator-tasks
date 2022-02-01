# coding: utf-8

# std
import re

# App
from v8_api_gen_r16258 import scalars
import inner_reuse_local


class VarDeclaration(object):
    class Type(object):
        def __init__(self, type_name):
            self.name = type_name

    def __init__(self, type_name, name):
        self.type = VarDeclaration.Type(type_name)
        self.name = name


class ExtractorVarsDeclarations(object):
    """ Возвращает строку, в которой содержится все пары тип + имя переменной

        class {
            // Work
            Type var;
            Type function(Type var);
            Type var[SOME];

            Type function(
                Type0 var);

            // Don't work
            Type<Type<
                Var> var;
            Type function(
                Type1 var,
                Type0 var);

            // May be not work
            Type function() {

            }
        }
    """

    def __init__(self):
        self.__MAX_DIMENSION = 2

    def __first_filtration(self, code_lines):

        result = []
        FILTER_REGEX = "bool""|int""|vector<""|string""|char"
        PATTERN = re.compile(FILTER_REGEX)
        #search_result = PATTERN.search(line)

        for line in code_lines:
            # Фильтрация кода
            # TODO: Может негенерить много ошибок
            # Можно внутри класса разбить так.
            # Сперва вытянуть в строку.
            # Затем разбить ;/:/ и только потом отфильтровать.
            if '(' not in line \
                and ")" not in line \
                and ";" in line \
                and "{" not in line \
                and "}" not in line \
                and "#" not in line \
                and "public:" not in line \
                and "private" not in line \
                and "protected" not in line:

                search_result = PATTERN.search(line)
                if search_result:
                    line_copy = line
                    line_copy = line_copy.lstrip().rstrip()
                    line_copy = inner_reuse_local.remove_cc_comments(line_copy)
                    line_copy = inner_reuse_local.delete_double_spaces(line_copy)
                    result.append(line_copy)

        return '\n'.join(result)

    def __remove_lr_spaces(self, string):
        return string.rstrip().lstrip()

    def __make_type_value_list(self, lines):
        declarations = ' '.join(lines).split(' ')  # hack!

        result = []
        var_type = ""
        # Bug was here
        for index, record in enumerate(declarations):
            record = self.__remove_lr_spaces(record)  # Да, эти лучше тоже отфильтровать
            if record:
                if index % 2:
                    var_name = record
                    if var_type and var_name:
                        result.append((var_type, var_name))
                else:
                    var_type = record
                    # Bug was here
        return result

    def __end_filtration(self, declaration_string):
        declaration_string = declaration_string \
            .replace('\t', " ") \
            .replace('\n\t', " ") \
            .replace("  ", " ") \
            .replace('\n', " ")
        declaration_string = inner_reuse_local.delete_double_spaces(declaration_string)
        return declaration_string

    def extract_var_declaration(self, source):
        code_lines = source.split('\n')
        declaration_string = self.__first_filtration(code_lines)

        # Похоже вся магия здесь
        folded_string = self.__remove_lr_spaces(self.__end_filtration(declaration_string))

        declarations = folded_string.split(';')
        declarations = map(self.__remove_lr_spaces, declarations)
        declarations = filter(lambda x: x, declarations)  # удаляем пустые строки
        declarations = filter(self.__is_need, declarations)

        # Похоже на итоговую запаковку
        type_name_list = self.__make_type_value_list(declarations)
        return type_name_list

    def __is_need(self, at):
        return not ('*' in at
                    or '=' in at
                    or 'const' in at
                    or 'static' in at
                    or at.count('[') > self.__MAX_DIMENSION)

    def extract_field_declarations(self, source, class_name='unknown'):
        """
        Args:
            source - string with code
        """
        type_and_var_list = self.extract_var_declaration(source)
        result = []
        for var in type_and_var_list:
            result.append(scalars.MakerV8ScalarFieldAccessor(class_name, VarDeclaration(*var)))

        return result