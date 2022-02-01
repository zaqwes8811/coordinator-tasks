# coding: utf-8
"""
Name *.exe file placed in EXE env. var.

Approx. count shardes in

Best if all code in one file. But if file make big best create lib and give it
to shardes.

run only modified Octo.py

"""

import sys
if sys.version_info[0:2] != (2, 6):
    raise Exception("Must use python 2.6")

# Std
import os
import subprocess

# App
from inner_reuse import _make_exe_file


def final(key, value):
    pass


def _parse_key(key):
    return key.split('@')[0], key.split('@')[1]


def _get_case_name(out):
    lines = out.split('\n')
    return out
    #yield out


def mapfn(key, value):
    
    # Extract data from key
    gtest_shard_index, gtest_total_shards = _parse_key(key)

    # Create execute file
    exe_file_name = _make_exe_file(gtest_shard_index, value)

    # Tune env for process
    local_env = os.environ.copy()
    local_env['GTEST_TOTAL_SHARDS'] = gtest_total_shards
    local_env['GTEST_SHARD_INDEX'] = gtest_shard_index

    #out, err = run_file(exe_file_name, local_env)
    out = "hello"
    generator = _get_case_name(out)
    
    # Можно собирать дополнительную информацию
    #for i in generator:
    #    yield i.decode('cp1251').encode('utf8'), 1

    yield generator.decode('cp1251').encode('utf8'), 1


def reducefn(key, value):
    return key, len(value)

