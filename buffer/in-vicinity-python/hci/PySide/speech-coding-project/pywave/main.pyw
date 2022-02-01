# -*- coding: utf-8 -*-
import sys
from PySide import QtGui, QtCore,  QtDeclarative
# Gui
from PySide.QtGui import QWidget, QSpinBox, QSlider, QHBoxLayout, QPushButton, QDial
from PySide.QtGui import QFileDialog

# Core
from PySide.QtCore import QObject, SIGNAL, SLOT, Qt
from PySide.QtCore import QFile

# Other
from PySide.QtUiTools import QUiLoader 


# MyModules
import wave_base as wb

# 
class Foo:
  pass
def someFunc():
  (a, b) = QFileDialog.getOpenFileName(None)
  print a
  wb.show_wav(a)  # ������ ���������
  
class MainForm:
  pass

# input point
def main():
  app = QtGui.QApplication(sys.argv)  # ������ �������� ������ ���������
  # Windows  �� ���������
  loader = QUiLoader()
  file = QFile("main_form.ui")
  file.open(QFile.ReadOnly)
  myWidget = loader.load(file)
  file.close()

  # button
  Acotor = QPushButton("Action")
  #Acotor.connect(SIGNAL('clicked()'), someFunc)
  myWidget.pushButton.connect(SIGNAL('clicked()'), someFunc)

  # ���������
  layout = QHBoxLayout()
  #layout.addWidget(Acotor)
  myWidget.setLayout(layout)
  
  # ����������
  myWidget.show()
  app.exec_()  # ��������� ����������
if __name__ == "__main__":
  sys.exit(main())
  
