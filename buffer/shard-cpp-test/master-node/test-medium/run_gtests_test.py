# coding: utf-8
import sys

if sys.version_info[0:2] != (2, 6):
    raise Exception("Must use python 2.6")

import os

# Inner Reuse
from inner_reuse.configurer import Configurer
from inner_reuse import tic, toc

# App
from _code.gtest_runner import run_gtests


RUNNER_TASK_FILE = os.sep.join(['_code', 'task_files', 'main_task.py'])
BIN_FILE_NAME = os.sep.join(['..', 'statistic', 'exec', 'gtest-extend.bin'])
count_cores = 4


def test_run_gtest_no_parallel():
    tic()
    print '\nWarning: Need run at least one shard...\n'

    generator = run_gtests(RUNNER_TASK_FILE, count_cores, BIN_FILE_NAME)
    toc()

    tic()
    # Really make job!
    for i in generator:
        a, b = i
        print a
    toc()

test_run_gtest_no_parallel()