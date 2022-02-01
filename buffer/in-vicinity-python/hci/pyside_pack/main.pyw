# -*- coding: utf-8 -*-
import sys
from PySide import QtGui, QtCore,  QtDeclarative
from PySide.QtGui import QWidget, QSpinBox, QSlider, QHBoxLayout, QPushButton, QDial
from PySide.QtCore import QObject, SIGNAL, SLOT, Qt
from PySide.QtUiTools import QUiLoader 
from PySide.QtCore import QFile

##
#import hello_ext


# native
#import matplotlib.pyplot as plt
#import numpy as np
#from matplotlib.pyplot import figure, show, rc, grid

def someFunc():#(val_int):
  pass
  #print hello_ext.greet()
  #print(val_int)
  #x = np.arange(0, 5, 0.3);
  #y = np.sin(x)
  #print(y)
  #plt.plot(x, y)
  #plt.show()
  # radar green, solid grid lines


def main():
  app = QtGui.QApplication(sys.argv)  # создаёт основной объект программы
  # Windows 
  # из дизайнера
  loader = QUiLoader()
  file = QFile("un.ui")
  file.open(QFile.ReadOnly)
  myWidget = loader.load(file)
  file.close()
  #
  wind = QWidget()
  wind.setWindowTitle("asdf")
  ###
  slider = QSpinBox() 
  slider.setRange(0, 130)
  spinbox = QSlider(Qt.Horizontal, None)
  spinbox.setRange(0, 130)
  spinbox.connect(SIGNAL('valueChanged(int)'), slider, SLOT('setValue(int)') )
  slider.connect(SIGNAL("valueChanged(int)"), spinbox, SLOT("setValue(int)"))

  # button
  but = QPushButton("action")
  but.connect(SIGNAL('clicked()'), someFunc)
  myWidget.dial.connect(SIGNAL('valueChanged(int)'), myWidget.lcdNumber, SLOT('display(int)'))
  # компануем
  layout = QHBoxLayout()
  layout.addWidget(spinbox)
  layout.addWidget(slider)
  layout.addWidget(but)
  #wind
  myWidget.setLayout(layout)
  
  # показываем
  #wind
  myWidget.show()
  #

  
  app.exec_()  # запускает приложение
if __name__ == "__main__":
  sys.exit(main())
  
