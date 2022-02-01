# coding: utf-8

import sys
if sys.version_info[0:2] != (2, 6):
    raise Exception("Must use python 2.6")

# 
import logging
import os

# App
import _code.octo_server_adapter as octo_server_adapter
from inner_reuse.configurer import Configurer


def init_logging_system():
    logging.basicConfig(logging=logging.INFO,
                    format="[%(levelname)s] %(message)s")
    logging.getLogger("").setLevel(logging.INFO)


if __name__ == '__main__':
    init_logging_system()
    path_to_script = sys.argv[0]
    # Need to know root master node
    head, tail = os.path.split(path_to_script)

    ''' ! Important. This const be depend form size cluster. '''
    count_cores = 4
    import time
    transition_process_duration = Configurer(
            head+os.sep+'master.yaml').get_int('cluster', 'delay_after_estimate')
    # Нужно время на восстановление
    time.sleep(transition_process_duration)  # Trouble!

    ''' Run tests MapReduce '''
    path_to_test = sys.argv[1]
    exe_file_name = path_to_test
    task_file = head+os.sep+'task_files/_run_tests_task.py'
    generator = _run_tests(task_file, count_cores, exe_file_name)
    
    # Get response
    for i in generator:
        print i



