# coding: utf-8
import subprocess
import time


def get_count_cores():
    """
      Important: not cross platrom maybe.
    """
    from multiprocessing import cpu_count
    return cpu_count()

def _stream_to_file(filename, stream):
    f = open(filename, 'wb')
    try:
        f.write(stream)
    finally:
        f.close()


def _make_exe_file(gtest_shard_index, stream):
    """
    Даже есть запущено несколько шардов, то один файл разом они писать не будут.
    """
    tmp_dir = 'tmp'
    exe_file_name = tmp_dir+os.sep+gtest_shard_index+'.exe'
    if not os.path.exists(tmp_dir):
        os.makedirs(tmp_dir)
    _stream_to_file(exe_file_name, stream)
    return exe_file_name


def tic():
    #Homemade version of matlab tic and toc functions
    import time
    global startTime_for_tictoc
    startTime_for_tictoc = time.time()
    

def toc():
    import time
    if 'startTime_for_tictoc' in globals():
        print "Elapsed time is " + str(time.time() - startTime_for_tictoc) + " seconds."
    else:
        print "Toc: start time not set"


def run_bin_with_wait(exe_file_name, local_env, wait_time=1):
    """
    Return:
        Накопившийся ответ.
    """
    TIMEOUT_POLLING = 0.3
    max_iteration = wait_time*1.0/TIMEOUT_POLLING
    max_iteration = int(max_iteration)

    p = subprocess.Popen([exe_file_name], env=local_env, stdout=subprocess.PIPE)
    i = 0
    try:
        while True:
            if not p.poll():
                break
            time.sleep(TIMEOUT_POLLING)
            i += 1
            if i > max_iteration:
                p.kill()
                break

    except WindowsError as e:
        print 'Error on kill'

    # Читаем что успели накопить
    return p.stdout.read()


def run_file(exe_file_name, local_env):
    p = subprocess.Popen([exe_file_name], stdout=subprocess.PIPE, env=local_env)
    out, error = p.communicate()
    return out, error


def chunks(l, n):
    """ Yield successive n-sized chunks from l.
    http://stackoverflow.com/questions/312443/how-do-you-split-a-list-into-evenly-sized-chunks-in-python
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]
