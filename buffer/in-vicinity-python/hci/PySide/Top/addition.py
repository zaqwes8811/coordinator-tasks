# -*- coding: utf-8 -*-
# file : addition.py
"""

Authors:
LI Ligansky Igor 

Changes:
3-Окт-11 : добавлен доступ к закрытым членам класса
"""
from PySide import QtCore
from PySide import QtGui
from PySide.QtGui import QFileDialog

# 
from pylab import *
import wave
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker  # 
import math
import scipy as sp

# MyImport
#import filters as firs

types = {  #  numpy
    1: np.int8,
    2: np.int16,
    4: np.int32
}
      

""" класс для обработки wav-файлов """
class waveProcessing(QtCore.QObject):
  # функции преобразования потока
  def pr(self, ichannel):  # 
    for i in range(10000):  # 
      ichannel[i] -= 1000
  
  # Property
  _fName = None  #
  _fNameOut = None 
  _nchannels = None
  _sampwidth = None # разрядность данных
  _framerate = None # частота дискретизации
  _nframes = None # число фрэймов
  _comptype = None
  _compname = None
  

  # Slots
  @QtCore.Slot()  # @ - что-то общеязыковое?
  def loadFile(self):
    (a, b) = QFileDialog.getOpenFileName(None)  # нужно фильтр сделать
    self._fName = a
    #print 
    self.parseParameters()
  
  # рисует все графики 
  @QtCore.Slot()
  def showWFandSG(self):
    # shaping
    #if(self.fName != "None")
    wav = wave.open(self._fName, mode="r")
    ( nchannels, 
      sampwidth,  # разрядность данных
      framerate,  # частота дискретизации
      nframes,  # число фрэймов
      comptype, compname
    ) = wav.getparams()
    content = wav.readframes(nframes)  # 
    samples = np.fromstring(content, dtype=types[sampwidth])  # 
    channel = samples[0::nchannels]  # 

    #
    NFFT = 512
    duration = 1/float(framerate)*nframes  # 

    # 
    ax1 = subplot(211)
    Pxx, freqs, bins, im = specgram(
      channel,  NFFT = NFFT, # (STFT) 
      Fs = framerate,  Fc = 0,  # 
      detrend=mlab.detrend_none,  window=mlab.window_hanning,  # 
      noverlap = NFFT*0.5,  pad_to = NFFT  # 
    )
 
    # поле экспериментов
    lench = len(channel)
    xtime = np.ndarray(lench)
    for i in range(lench):  # 
      xtime[i] = (i*duration)/lench
    
    subplot(212)  
    plot(xtime, channel)   
    plt.grid(True, color="g")
    
    xs = 20480
    axis([0, xtime[lench-1],-xs, xs])
    show() 
  
  # ? как вернуть все разом (типа как в матлабе)
  def parseParameters(self):
    wav = wave.open(self._fName, mode="r")
    ( self._nchannels,  self._sampwidth,  # 
      self._framerate,  self._nframes,  # 
      self._comptype, self._compname
    ) = wav.getparams()
    return [self._nchannels, self._framerate, self._nframes]

  # Доступ к свойствам
  @QtCore.Slot(result = int)  # число каналов
  def getNumChannals(self):
    return self._nchannels
  @QtCore.Slot(result = int)  # разрядность данных
  def getSampWidth(self):
    return self._sampwidth
  @QtCore.Slot(result = int)  # частота дискретизации
  def getFrameRate(self):
    return self._framerate
  @QtCore.Slot(result = int)  # число отсчетов
  def getNumFrames(self):
    return self._nframes
