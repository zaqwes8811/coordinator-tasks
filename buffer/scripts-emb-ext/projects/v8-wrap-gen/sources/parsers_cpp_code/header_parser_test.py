# coding: utf-8
import random
import unittest

# Unit
from parsers_cpp_code import header_handmade_parser

# Заголовок таки должен компилироваться,
#   поэтому откровенного бреда там быть не должно
#
# Файл реализации полностью выделен
# Вложенных классов нет
# Забивает на модификаторы доступа - переводит класс в структуру.
#
# Директивы предпроцессора отбрасывает. Из-за сложность реализации.
#   Можно пропустить файл через предпроцессор, что отфильтрует задефайненное.
SIMPLE_CLASS = """
class Foo {\n
private:\n
\n
public:\n
    int  i;\n
    int get(\n
        int k,\n
        int j);\n
    int array[10];\n
    int array[10][10];
    Point array[9];\n
};\n
"""


class CppClassReformer(object):
    """
    Убирает выравнивание кода
    """

    def reform(self, source):
        pass

class TestExtractorVarsDeclarations(unittest.TestCase):
    def test_shuffle(self):
        extractor = header_handmade_parser.ExtractorVarsDeclarations()
        declarations = extractor.extract_var_declaration(SIMPLE_CLASS)
        print declarations

    def test_reformer(self):
        pass



