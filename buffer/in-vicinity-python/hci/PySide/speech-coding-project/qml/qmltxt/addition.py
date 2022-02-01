# -*- coding: utf-8 -*-
# file : addition.py
from PySide import QtCore

# 
from pylab import *
import wave
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker  # форматирование временных меток
import math
import scipy as sp

# MyImport
import filters as firs

types = {  # из numpy
    1: np.int8,
    2: np.int16,
    4: np.int32
}
	  

# _TEST_
class Console(QtCore.QObject):
  # def __init__(self, parent=None):
   # pass

  def pr(self, ichannel):  # по ссылке похоже передает
    for i in range(10000):  # ось
      ichannel[i] -= 1000
  
  # #
  @QtCore.Slot(str)
  def outputStr(self, s):
    print s
	# shaping
    wav = wave.open("myspeech.wav", mode="r")
    (
    nchannels, 
    sampwidth,  # разрядность данных
    framerate,  # частота дискретизации
    nframes,  # число отсчетов 
    comptype, compname
    ) = wav.getparams()

    print  nchannels, sampwidth, framerate, nframes
    content = wav.readframes(nframes)  # читаем сразу все
    samples = np.fromstring(content, dtype=types[sampwidth])  # используется структ. types
    channel = samples[0::nchannels]  # разборка по каналам
	# теперь фильтруем
    self.pr(channel)
	#
    NFFT = 512
    duration = 1/float(framerate)*nframes  # расстойние между отсчетами
    print duration

    # теперь рисуем
    # спектрограмма
    ax1 = subplot(211)
    Pxx, freqs, bins, im = specgram(
    channel,  # данные для построения
    NFFT = NFFT, # размер окна (STFT) 
    Fs = 44100,  # частота дискретизации
    Fc = 0,  # центерльная частота
    detrend=mlab.detrend_none,  # ?
    window=mlab.window_hanning,  # окно для преобразования
    noverlap = 64,  # на сколько перекрываются выборки
    pad_to = NFFT  # дополнение для ? увеличения разрешения
    )
 
    # временное представление
    lench = len(channel)
    xtime = np.ndarray(lench)
    for i in range(lench):  # ось
      xtime[i] = (i*duration)/lench*10
    xs = 2048
    yval = sinc(xtime)
	# window_hanning(), window_none(), numpy.blackman(), numpy.hamming(), numpy.bartlett(),mlab.window_hanning(xtime)
    yval = mlab.window_none(xtime)  #sp.fft(yval)
    yval *= sinc(xtime)
    yval = abs(sp.fft(yval))
    subplot(212)  
    plot(xtime, yval)   
    plt.grid(True, color="g")
    
    #axis([0, xtime[lench-1],-xs, xs])
    show() 
  
  # показать параметры загруженного файла
  @QtCore.Slot(result = int)
  def showParam(self):
    wav = wave.open("myspeech.wav", mode="r")
    (
    nchannels, 
    sampwidth,  # разрядность данных
    framerate,  # частота дискретизации
    nframes,  # число отсчетов 
    comptype, compname
    ) = wav.getparams()

    return nchannels  #, framerate, nframes)
  
	