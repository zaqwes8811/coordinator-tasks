from _code.octo_server_adapter import do_customize_server


def run_gtests(task_file, count_cores, exe_file_name):
    def _file_to_stream(filename):
        f = open(filename, 'rb')
        try:
            return f.read()
        finally:
            f.close()
    
    stream = _file_to_stream(exe_file_name)
    
    # Create jobs
    keys = []
    timeout = 0
    for i in range(count_cores):
        keys.append(str(i)+'@'+str(count_cores)+'@'+str(timeout))
    
    jobs = dict((key, stream) for key in keys)
    return do_customize_server(task_file, jobs, type='simple')