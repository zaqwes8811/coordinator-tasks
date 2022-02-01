﻿# -*- coding: utf-8 -*-
import sys
from PySide import QtCore, QtGui, QtDeclarative
from PySide.QtCore import QObject, SIGNAL, SLOT, Qt
from PySide.QtGui import QFileDialog

# MyImport
from addition import *
class MyClass:
  pass
  


# ����� �����
def main():
  app = QtGui.QApplication(sys.argv)

  # �������� QML �����
  view = QtDeclarative.QDeclarativeView()
  view.setSource(QtCore.QUrl("form.qml"))
  view.setResizeMode(QtDeclarative.QDeclarativeView.SizeRootObjectToView)
  view.show()
  
  # �������� ������� � QML
  con = Console()
  context = view.rootContext()
  context.setContextProperty("con", con)
  
  # ������
  app.exec_() 
if __name__ == "__main__":
  sys.exit(main())
