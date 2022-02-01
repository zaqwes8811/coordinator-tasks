# -*- coding: utf-8 -*-
# file : addition.py
from PySide import QtCore

# _TEST_
class Console(QtCore.QObject):
  @QtCore.Slot(str)
  def outputStr(self, s):
    print s