# coding: utf-8

"""
Args:
    exe file name - 1
    work dir - 
    
    VSEE2010 example  - $(TargetPath) $(SolutionDir)

Task:
  Очистить выходные данные для Google Test.
"""

import sys
import os

from inner_reuse import run_bin_with_wait
from inner_reuse.out_parser import parser_out
from inner_reuse.reporter import report_gtest_result
from inner_reuse.configurer import Configurer
from inner_reuse import tic, toc

from launchers import mp_launcher

EXE_FILE_NAME_IDX = 1
SCRIPT_FILE_NAME_IDX = 0
WORK_DIR_IDX = 2


def run_one_process(exe_file_name, how_much_wait_result):
    # Запускаем
    local_env = os.environ.copy()
    tic()
    # Около 0.3.. sec! на загрузку выполнение, а по факту на загрузку *.bin
    out = run_bin_with_wait(exe_file_name, local_env, how_much_wait_result)
    toc()
    result = parser_out(out)
    report_gtest_result(result)


def main(argv, launcher):
    # Read configuration
    script_work_dir, tail = os.path.split((argv[SCRIPT_FILE_NAME_IDX]))
    path_to_cfg = script_work_dir + os.sep + 'direct-launcher.yaml'
    how_much_wait_result = Configurer(path_to_cfg)\
        .get_double('how_much_wait_result')

    # Get bin file
    exe_file_name = sys.argv[EXE_FILE_NAME_IDX]
    work_dir = sys.argv[WORK_DIR_IDX]

    # Переходим в папку с исполняемым файлом
    head, tail = os.path.split(exe_file_name)
    os.chdir(head)

    #print 'Work dir:', os.getcwd()
    print 'Wait result: ', how_much_wait_result, ' sec. For change this edit ', path_to_cfg

    # !!
    os.chdir(work_dir)
    print 'Work dir:', os.getcwd()

    launcher(exe_file_name, how_much_wait_result)

if __name__ == '__main__':
    #tic()
    #main(sys.argv, mp_launcher)
    #toc()

    tic()
    main(sys.argv, run_one_process)
    toc()

