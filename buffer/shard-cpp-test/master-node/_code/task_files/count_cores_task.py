# coding: utf-8
"""
    Нужно зацепится за информацию, уникальную для машины.
    Пусть пока это будет MAC-адрес.
"""


# Summator?
def final(key, value):
    pass


# client
def mapfn(key, value): 
    import multiprocessing
    from uuid import getnode as get_mac
    # Можно собирать дополнительную информацию
    count_cores = multiprocessing.cpu_count()
    key2 = str(get_mac())
    yield key2, count_cores


def reducefn(key, value):
    # Берем только первый мак-адрес
    return key, list(set(value))[0] #len(value)
