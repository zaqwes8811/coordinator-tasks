# coding: utf-8
import sys
if sys.version_info[0:2] != (2, 6):
    raise Exception("Must use python 2.6")

import logging

from multiprocessing import Pool
from multiprocessing import Process
from multiprocessing import cpu_count

# Other
from _code.octo_client_adapter import do_client
from inner_reuse.configurer import Configurer


def shardfn(ip_master):
    """ Время работы не должно превышать некоторую величину заданную в
        настройках пула процессов. """
    task_file, type, options = ip_master, 'simple', {}  # it's strange!
    # Независимыми сделать не получается - есть таймаут на выполнение операции
    while True:
        try:
            do_client(taskfile=task_file, type=type, **options)
            import time
            time.sleep(1)
        except Exception as e:
            # Ловим все! Устойчивость важна
            pass
            logging.error(e)
            print e


if __name__ == '__main__':
    logging.basicConfig(logging=logging.INFO, format="[%(levelname)s] %(message)s")
    logging.getLogger("").setLevel(logging.INFO)

    file_name = 'shard.yaml'
    path = 'ips-master-node'
    ip_masters = Configurer(file_name).get_list(path)
    ip_only_one_master = ip_masters[0]

    # Нужно создать список адресов
    num_processes = cpu_count()
    ips = []
    for i in range(num_processes):
        ips.append(ip_only_one_master)

    # Запускаем шарды
    # Pool похоже не очень подходит. Async_map похоже подходит для обработки связанных
    #   данных. Возможно это увеличит время переходных процессов. Хотя если зациклить, то может
    #   все и норм будет.
    #'''
    #while True:  # From keyboard interrupt no help
    #    try:
    pool = Pool(processes=num_processes)
    result = pool.map_async(shardfn, ips)
    result.get()  # timeout=10)
    #    except Exception as e:
    #        pass
    #'''

    # Для каждого IP процесс будет один, а в mapfn будем параллелить - нет!
    #   будет плохая балансировка. Но подходить для больщих тестовых файлов, т.к. каждому
    #   компьютеру нужно передавать только один экземпляр.
    '''p = Process(target=shardfn, args=(ip_only_one_master,))
    p.start()
    p.join()'''
