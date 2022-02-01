# coding: utf-8

import sys
import unittest

from generator.cpp import utils

from v8_api_gen_r16258 import scalars
from parsers_cpp_code import header_handmade_parser
import inner_reuse_local


class TestExtractorFirstLevel(unittest.TestCase):
    def test_shuffle(self):
        header_file_name = '../tests/data/sampler.h'
        source = utils.ReadFile(header_file_name)
        class_name = "SomeThing"

        #dec_wrappers = header_handmade_parser.extract_variable_declaration_own(source, class_name)
        #code = scalars.do_scalar_setter_impl(dec_wrappers)
        #for line in code:
        #    print line


        '''
        header_name = 'odata/forge_v8_point.h'
        utils_local.write_source(header_name, code)

        code = scalars.make_source(impls, header_name)

        # Итоговый исходник
        utils_local.write_source('odata/forge_v8_point.cc', code)'''

