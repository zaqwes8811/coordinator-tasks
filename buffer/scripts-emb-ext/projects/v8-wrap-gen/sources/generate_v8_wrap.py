# coding: utf-8

# std
import sys

# App
from generators_cpp_code import maker_sources

if __name__ == '__main__':
    header_to_wrap = sys.argv[1]
    maker_sources.make_complect(header_to_wrap)
