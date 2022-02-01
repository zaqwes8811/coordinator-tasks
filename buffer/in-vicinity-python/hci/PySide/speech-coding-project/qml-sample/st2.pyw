# -*- coding: utf-8 -*-
import sys
from PySide import QtGui, QtCore,  QtDeclarative
from PySide.QtGui import QWidget, QSpinBox, QSlider, QHBoxLayout, QPushButton, QDial
from PySide.QtCore import QObject, SIGNAL, SLOT, Qt
from PySide.QtUiTools import QUiLoader 
from PySide.QtCore import QFile

def someFunc():#(val_int):
  pass
  #print(val_int)

def main():
  app = QtGui.QApplication(sys.argv)  # создаёт основной объект программы
  # создается здесь
  # из дизайнера

  # из QML
  
  view = QtDeclarative.QDeclarativeView()
  view.setSource(QtCore.QUrl('behavior-example.qml'))#form.qml'))
  view.setResizeMode(QtDeclarative.QDeclarativeView.SizeRootObjectToView)
  view.setGeometry(100, 100, 400, 240)
  view.show()
  
  # на запуск
  app.exec_()  # запускает приложение
if __name__ == "__main__":
  sys.exit(main())
  
