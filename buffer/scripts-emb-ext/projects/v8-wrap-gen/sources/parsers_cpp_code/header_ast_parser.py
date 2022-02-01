# coding: utf-8

"""

Constraints:
    В файле один класс. Это связано с тем, что парсер из cppclean не понимает массивы в загол.
    файлах, поэтому он реализован отдельно и при текущем алгоритме работы границы классов
    игнорируются - парсер будет все массивы что встретит будет считать массивами одного класса.
"""

# Std
import re

# Third_party
from generator.cpp import ast


class ArrayXD(object):
    def __init__(self, type_var, dim_var, name_var):
        self.type_var_ = type_var
        self.dim_var = dim_var  # Хранить в строковом виде, т.к. размерностью могут быть константы
        self.name_var = name_var

    def XXX__str__(self, name):
        return '%s(%d, %d, %s %s)' % (name, 0, 0, self.type_var_, self.dim_var)

    def __repr__(self):
        return self.XXX__str__(self.__class__.__name__)


class ASTHeaderParser(object):
    def __init__(self, source, header_file_name):
        self.header_file_name_ = header_file_name
        self.source_ = source

    def extract_declarations(self):
        """
        Abstract:
            Extract declaration form C++ header file.

        Args:
            source - content header file
            header_file_name - name header file

        Returns:
            {class_name [declarations]]
        """
        result = []
        builder = ast.BuilderFromSource(self.source_, self.header_file_name_)
        class_name = ''
        occur = False
        for node in builder.Generate():
            if isinstance(node, ast.Class):
                if not occur:
                    class_name = node.FullName()
                    occur = True
                else:
                    raise Exception('Error: No one class in header file.')
                for record in node.body:
                    if isinstance(record, ast.VariableDeclaration):
                        if '???' not in record.FullName():
                            result.append((class_name, record))
                    if isinstance(record, ast.Function):
                        result.append((class_name, record))
        if not occur:
            raise Exception('Error: No one class found.')

        # Добавляем массивы
        # TODO: very dirty.
        FILTER_REGEX_NAME = " [^\s]*?\["
        FILTER_RE_TYPE = '.*? '
        FILTER_RE_DIM = '\[[^[]\]'
        source_lines = self.source_.split('\n')
        for line in source_lines:
            if '[' in line and '(' not in line:
                declaration = line.lstrip().rstrip()
                name = re.compile(FILTER_REGEX_NAME).search(declaration).group()
                name = name[1:-1]
                type_var = re.compile(FILTER_RE_TYPE).search(declaration).group()
                type_var = type_var[:-1]
                dims_string = re.compile(FILTER_RE_DIM).findall(declaration)
                dim_var = map(lambda record_var: record_var.replace('[', '').replace(']', ''), dims_string)
                record = ast.VariableDeclaration(start=0, end=0, name=name,
                                                 var_type=ArrayXD(type_var, dim_var, name), initial_value=0,
                                                 namespace='unknown')
                result.append((class_name, record))
        return result
