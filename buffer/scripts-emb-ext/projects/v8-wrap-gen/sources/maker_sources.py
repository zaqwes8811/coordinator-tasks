# coding: utf-8

from os.path import basename
import os

from generator.cpp import utils

from v8_api_gen_r16258 import wrapper_builder
import inner_reuse_local
from parsers_cpp_code import header_handmade_parser


def get_declarations_from_header(source_code):
    type_and_var_list = header_handmade_parser.ExtractorVarsDeclarations().extract_var_declaration(source_code)
    result = []
    class_name = "RefineInMemoryStorage"

    for record in type_and_var_list:
        updated = list(record)
        updated.append(class_name)
        type_var = updated[0]
        bad = 'vector' in updated[0] or \
              'string' in updated[0] or \
              'static' in updated[0] or \
              '=' in updated[0] or \
              '*' in type_var
        if not bad:
            result.append(updated)
    return result, class_name


def make_header_file(header_name, class_name, builder, header_to_wrap):
    header_name_tmp = header_name+'.h'
    directive = header_name_tmp.replace('/', '_').replace('\\', '_').replace('.', '_').upper() + '_'

    # header
    code = []
    code.append('#ifndef ' + directive)
    code.append('#define ' + directive)
    code.append('')
    code.append('// Other')
    code.append('#include <v8.h>')
    code.append('')
    code.append('#include "'+header_to_wrap+'"')
    code.append('')
    code.append('namespace app_server_scope {')
    code.append('class ' + class_name + 'V8 {')
    code.append(' public:')
    code.append(builder.blueprint_method_decl())
    code.append('')
    code.append(builder.new_method_decl())
    code.append('')
    code.append('  // Vector accessors')
    for impl in builder.get_vector_decls():
        code.append(impl)

    code.append('  // Scalar accessors')
    for impl in builder.get_scalar_decls():
        code.append(impl)

    # Static
    code.append('};')
    code.append('}')
    code.append('#endif  // ' + directive)
    return code


def make_source_file(pair, builder):
    code = []
    code.append('#include "'+pair+'"')
    code.append('#include "third_party/v8/src/process.h"')
    code.append('')
    code.append('using v8::String;')
    code.append('using v8::ObjectTemplate;')
    code.append('using v8::Object;')
    code.append('using v8::HandleScope;')
    code.append('using v8::Handle;')
    code.append('using v8::Local;')
    code.append('using v8::Value;')
    code.append('using v8::External;')
    code.append('using v8::Isolate;')
    code.append('using v8::Number;')
    code.append('using v8::Undefined;')
    code.append('using v8::Context;')
    code.append('using v8::Integer;')

    code.append('')
    code.append('namespace app_server_scope {')

    code.append(builder.blueprint_method_impl())
    code.append('')
    code.append(builder.new_method_impl())

    code.append('// Vector accessors')
    for impl in builder.get_vector_impls():
        code.append(impl)


    code.append('  // Scalar accessors')
    for impl in builder.get_scalar_impls():
        code.append(impl)

    code.append('}')
    return code


def make_complect(header_to_wrap):
    code = utils.ReadFile(header_to_wrap)
    declarations, class_name = get_declarations_from_header(code)

    # Names
    dir_name = os.path.dirname(header_to_wrap)
    header_name = basename(header_to_wrap)
    header_no_ext = header_name.split(os.extsep)[0]
    v8_header = header_no_ext + '_v8_auto_gen_r16258'

    # Targets
    if dir_name:
        pair_name = os.sep.join((dir_name, v8_header))
    else:
        pair_name = v8_header

    builder = wrapper_builder.BuilderV8AccessorsPackage(declarations, code)
    code = make_header_file(pair_name, class_name, builder, header_name)
    print pair_name+'.h'
    inner_reuse_local.write_source(pair_name+'.h', code)

    # source
    code = make_source_file(v8_header+'.h', builder)
    inner_reuse_local.write_source(pair_name+'.cc', code)