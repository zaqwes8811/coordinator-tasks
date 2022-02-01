# coding: utf-8
import time

# Other
from octo import prepare_taskfile
from octo import IPC_HANDLERS


def do_customize_server(task_file, jobs, type, transition_process_duration=0, **options):
    time.sleep(transition_process_duration)

    """ Starts the server, for extraction data form file. Added zaqwes8811. """
    task_module = prepare_taskfile(task_file)
    # prepare tasks
    tasks = jobs
    # start task/data server
    task_server = IPC_HANDLERS[type][0](tasks, task_file, **options)
    task_server.start()
    # tasks completed, run final
    if hasattr(task_module, "final"):
        for key in sorted(list(task_server.final.keys())):
            yield key, task_server.final[key]

