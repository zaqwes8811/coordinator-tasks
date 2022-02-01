# coding: utf-8
__author__ = 'кей'


# sum
def final(key, value):
    pass


# client
def mapfn(key, value):
    key2 = "hello"
    yield key2, 1


def reducefn(key, value):
    return key, len(value)