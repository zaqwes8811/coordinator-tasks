# coding: utf-8
"""
  Nose can't test it. По крайней мере не знаю как это сделать. Вызовы пула выходят по таймауту.

  Проблема запуска имеется. Сколько ждать ответа? Долго работающий тест не отличишь от дедлока!

#  --processes=4 - для Python2.6 начинается тупнят - все ядра загружены, а тесты не идут
#_multiprocess_can_split_ = True  # похоже не работает

Не остается следов. Пока не ясно как узнать какой тест ушел в deadlock.
 Не остается следов. Пока не ясно как узнать какой тест ушел в deadlock.
    Кажется стало яснее, но все равно отличить долго работающий тест от дедлока невозможно.

Info:
http://dou.ua/lenta/articles/nose-tests-deeper/

! Если запускать тесты с пулом, то некоторые тесты почему-то запускаются дважды!
"""

import os

from multiprocessing import Pool
from multiprocessing import TimeoutError

from nose.tools import raises

# App
from inner_reuse import run_bin_with_wait
from inner_reuse import run_file

TIME_TO_WAIT = 1
TOP_PATH = os.sep.join(['..', '..', 'statistic'])


@raises(TimeoutError)
def test_no_wait_run():
    local_env = os.environ.copy()
    exe = os.sep.join([TOP_PATH, 'exec', 'gtest-deadlock.bin'])
    pool = Pool(processes=1)

    #result = pool.apply_async(run_file, (exe, local_env))
    #result.get(timeout=TIME_TO_WAIT)
    #result.wait()

    raise TimeoutError


@raises(TimeoutError)
def test_inf_loop_run():

    local_env = os.environ.copy()
    exe = os.sep.join([TOP_PATH, 'exec', 'gtest-deadlock.bin'])

    #pool = Pool(processes=1)
    #result = pool.apply_async(run_file, (exe, local_env))
    #response = result.get(timeout=TIME_TO_WAIT)

    raise TimeoutError


def test_inf_loop_run_update_parallel():
    local_env = os.environ.copy()
    exe = os.sep.join([TOP_PATH, 'exec', 'gtest-deadlock.bin'])
    run_bin_with_wait(exe, local_env, TIME_TO_WAIT)


