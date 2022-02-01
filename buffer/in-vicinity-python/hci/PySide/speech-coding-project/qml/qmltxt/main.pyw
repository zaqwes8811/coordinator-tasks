# -*- coding: utf-8 -*-
import sys
from PySide import QtCore, QtGui, QtDeclarative
from PySide.QtCore import QObject, SIGNAL, SLOT, Qt

# MyImport
from addition import *

# точка входа
def main():
  app = QtGui.QApplication(sys.argv)

  # Создание QML формы
  view = QtDeclarative.QDeclarativeView()
  view.setSource(QtCore.QUrl('form.qml'))
  view.setResizeMode(QtDeclarative.QDeclarativeView.SizeRootObjectToView)
  view.setGeometry(100, 100, 250, 80)
  view.show()
  
  # передача объекта в QML
  con = Console()
  context = view.rootContext()
  context.setContextProperty("con", con)
  
  # запуск
  app.exec_() 
if __name__ == "__main__":
  sys.exit(main())
