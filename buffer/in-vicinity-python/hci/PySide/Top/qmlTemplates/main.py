#!/bin/python
# -*- coding: utf-8 -*-
""" 
  file : main.py

 """
import sys
from PySide import QtCore, QtGui, QtDeclarative
from PySide.QtCore import QObject, SIGNAL, SLOT, Qt
from PySide.QtGui import QFileDialog

# Main()
def main():
  app = QtGui.QApplication(sys.argv)  

  # QML view
  view = QtDeclarative.QDeclarativeView()  # тоже виджет! наследник
  view.setSource(QtCore.QUrl("UiBoard.qml"))
  view.setResizeMode(QtDeclarative.QDeclarativeView.SizeRootObjectToView)
  view.show()
  
  # ^запуск цикла обработки событий
  app.exec_() 
if __name__ == "__main__":
  sys.exit(main())
