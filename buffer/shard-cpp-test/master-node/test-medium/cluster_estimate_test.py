# coding: utf-8
import sys

if sys.version_info[0:2] != (2, 6):
    raise Exception("Must use python 2.6")

import os

# Other
import nose

# Inner Reuse
from inner_reuse.configure import Configurer

# App
from _code.cluster_estimator import get_count_cores


def test_estimation_no_parallel():
    print '\nWarning: Need run at least one shard...\n'
    task_file = '..' + os.sep + 'task_files' + os.sep + 'count_cores_task.py'
    path_to_config_file = '..' + os.sep + 'master.yaml'
    count_core_roughly = Configurer(path_to_config_file).get_int('cluster', 'count_core_roughly')
    count_cores = get_count_cores(
        task_file,
        count_core_roughly,
        transition_process_duration=1.0)
    assert count_cores > 0



