# coding: utf-8
__author__ = 'кей'

import sys

if sys.version_info[0:2] != (2, 6):
    raise Exception("Must use python 2.6")

import os
import logging

# App
from _code.octo_server_adapter import do_customize_server


def test_base_mapreduce_no_parallel():
    print '\nWarning: Need run at least one shard...\n'

    logging.basicConfig(logging=logging.INFO, format="[%(levelname)s] %(message)s")
    logging.getLogger("").setLevel(logging.INFO)

    # Task file and count jobs
    task_file = 'tasks' + os.sep + 'simple_mapreduce_task.py'
    count_jobs = 4

    # Make jobs
    keys = []
    for i in range(count_jobs):
        keys.append(str(i))
    jobs = dict((key, " ") for key in keys)

    # Run
    generator = do_customize_server(
        task_file,
        jobs,
        type='simple',
        transition_process_duration=1.0)

    # Process response
    result = 0
    for i in generator:
        key, (mac, cores) = i
        result += cores

    assert result == count_jobs


if __name__ == '__main__':
    test_base_mapreduce()