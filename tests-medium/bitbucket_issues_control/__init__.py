# coding: utf-8
__author__ = 'кей'

import requests


class Accessor(object):
    def __init__(self, username, password):
        self.username_ = username
        self.password_ = password
