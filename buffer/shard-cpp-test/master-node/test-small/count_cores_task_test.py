# coding: utf-8

import unittest

from _code.task_files.count_cores_task import mapfn


class TestScanerMapReduce(unittest.TestCase):
    def test_read_unic_pc_id(self):
        generator = mapfn(key=None, value=None)
        for i in generator:
            #self.assertGreater(i[1], 0)  # >= 2.7
            assert i[1] > 0
