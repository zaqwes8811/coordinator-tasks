# coding: utf-8

import unittest

from generator.cpp import utils
from generator.cpp import ast

from parsers_cpp_code import header_ast_parser


class TestAst(unittest.TestCase):
    def testMain(self):
        filename = '../tests/data/up2D.h'
        source = utils.ReadFile(filename, 'r')

        # Можно выделить все кроме 2D-массивов
        print header_ast_parser.ASTHeaderParser(source, filename).extract_declarations()
