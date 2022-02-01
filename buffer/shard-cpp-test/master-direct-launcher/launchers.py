# coding: utf-8

from multiprocessing import Pool
import os

from multiprocessing import Pool

from inner_reuse import get_count_cores
from inner_reuse import run_bin_with_wait
from inner_reuse.out_parser import parse_out_not_gen
from inner_reuse.reporter import report_gtest_result
from inner_reuse import run_bin_with_wait
from inner_reuse.out_parser import parse_out_not_gen

from inner_reuse import tic, toc


def f(x):
    # Run tests
   # print x
    local_env = os.environ.copy()
    local_env['GTEST_TOTAL_SHARDS'] = str(x[0])
    local_env['GTEST_SHARD_INDEX'] = str(x[1])
    exe_local = x[2]
    time_to_wait = x[3]

    # kill!! портим вызов!
    tic()
    out = run_bin_with_wait(exe_local, local_env, time_to_wait)
    toc()
    report = parse_out_not_gen(out)
    return report

def mp_launcher(exe, wait_delay):
    # Get count cores
    cores = get_count_cores()

    # Pack for s
    data = []
    for i in range(cores):
        data.append((cores, i, exe, wait_delay))
    pool = Pool(processes=cores)              # start 4 worker processes
    result = pool.map(f, data)    # evaluate "f(10)" asynchronously
    report_full = []
    for i in result:
        report_full.extend(i)

    # Объекдиняем результаты
    report_gtest_result(report_full)
