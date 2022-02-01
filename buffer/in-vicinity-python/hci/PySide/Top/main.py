# -*- coding: utf-8 -*-
""" 
  file : main.pyw
  ques. :
    вот здесь похоже можно соединить слот(qml) и сигнал(python)
    как сымитировать сигнал? emit?
    in the QWidget вроде точно так можно  
    обратное соединение ясно
 """
import sys
from PySide import QtCore, QtGui, QtDeclarative
from PySide.QtCore import QObject, SIGNAL, SLOT, Qt
from PySide.QtGui import QFileDialog

# MyImport
from addition import *


# Main()
def main():
  app = QtGui.QApplication(sys.argv)

  # QML view
  view = QtDeclarative.QDeclarativeView()  # тоже виджет! наследник
  view.setSource(QtCore.QUrl("form.qml"))
  view.setResizeMode(QtDeclarative.QDeclarativeView.SizeRootObjectToView)
  view.show()
  
  # передача объекта в QML-вид
  wProc = waveProcessing()  # класс для работы с wav-файлами
  context = view.rootContext()
  context.setContextProperty("wProc", wProc)
  
  # как я понимаю запуск ожидания событий
  app.exec_() 
if __name__ == "__main__":
  sys.exit(main())
