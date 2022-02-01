# coding: utf-8

import os


from inner_reuse import run_bin_with_wait
from inner_reuse.out_parser import parser_out
from inner_reuse.reporter import report_gtest_result

TIME_TO_WAIN = 2  # sec.
TOP_PATH = os.sep.join(['..', '..', 'statistic'])


def test_reporter():
    local_env = os.environ.copy()
    exe = os.sep.join([TOP_PATH, 'exec', 'gtest-extend.bin'])
    print 'Bin-filename: ', exe
    out = run_bin_with_wait(exe, local_env, TIME_TO_WAIN)
    result = parser_out(out)
    report_gtest_result(result)





