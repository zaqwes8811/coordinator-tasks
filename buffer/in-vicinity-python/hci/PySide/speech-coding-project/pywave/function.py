# -*- coding: utf-8 -*-
# native
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.pyplot import figure, show, rc, grid
# fucction
def someFunc():  #(val_int):
  pass
  #print hello_ext.greet()
  print("clicked")
  x = np.arange(0, 10, 0.05);
  y = np.sin(4*x)
  e = np.fft.fft(y)
  print(e)
  plt.plot(x, 20*np.log10(np.abs(e)))
  plt.show()
  #radar green, solid grid lines