# coding: utf-8
__author__ = 'zaqwes_user'

import os

from launchers import mp_launcher

TOP_PATH = os.sep.join(['..', 'statistic'])
TIME_TO_WAIT = 1


def main(launcher):
    exe = os.sep.join([TOP_PATH, 'exec', 'gtest-deadlock.bin'])
    launcher(exe, TIME_TO_WAIT)


if __name__ == '__main__':
    main(mp_launcher)
