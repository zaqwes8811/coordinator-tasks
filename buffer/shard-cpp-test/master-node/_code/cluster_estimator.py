# coding: utf-8
__author__ = 'кей'

from _code.octo_server_adapter import do_customize_server


def get_count_cores(task_file, count_core_roughly, transition_process_duration=0):
    # Make jobs
    keys = []
    for i in range(count_core_roughly):
        keys.append(str(i))
    jobs = dict((key, "") for key in keys)

    # Run
    generator = do_customize_server(task_file, jobs, type='simple')

    # Process response
    sum_cores = 0
    for i in generator:
        key, (mac, cores) = i
        sum_cores += cores

    return sum_cores
