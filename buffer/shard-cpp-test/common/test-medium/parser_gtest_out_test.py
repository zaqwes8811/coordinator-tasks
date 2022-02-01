# coding: utf-8
import os

from inner_reuse import run_bin_with_wait
from inner_reuse.out_parser import parser_out

TIME_TO_WAIN = 2  # sec.

TOP_PATH = os.sep.join(['..', '..', 'statistic'])


def test_run_from_file():
    file_name = os.sep.join([TOP_PATH, 'one-result.txt'])
    out = open(file_name).readlines()
    out = ''.join(out)
    result = parser_out(out)
    i = 0
    for j in result:
        i += 1
    assert i == 2


def test_run_deadlock_bin():
    local_env = os.environ.copy()
    exe = os.sep.join([TOP_PATH, 'exec', 'gtest-deadlock.bin'])
    out = run_bin_with_wait(exe, local_env, TIME_TO_WAIN)
    result = parser_out(out)
    i = 0
    for j in result:
        i += 1
    assert i == 7


def test_run_small_set_bin():
    local_env = os.environ.copy()
    exe = os.sep.join([TOP_PATH, 'exec', 'gtest-small-set.bin'])
    out = run_bin_with_wait(exe, local_env, TIME_TO_WAIN)
    result = parser_out(out)
    i = 0
    for j in result:
        i += 1
    assert i == 3


def test_run_extend_bin():
    local_env = os.environ.copy()
    exe = os.sep.join([TOP_PATH, 'exec', 'gtest-extend.bin'])
    out = run_bin_with_wait(exe, local_env, TIME_TO_WAIN)
    result = parser_out(out)
    i = 0
    for j in result:
        i += 1
    assert i == 8

