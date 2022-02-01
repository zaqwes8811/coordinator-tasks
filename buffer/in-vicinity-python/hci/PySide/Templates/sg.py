#!/usr/bin/env python
# -*- coding: utf-8 -*-
from pylab import *

import wave
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker  # форматирование временных меток
import math

types = {  # из numpy
    1: np.int8,
    2: np.int16,
    4: np.int32
}

# Pxx is the segments x freqs array of instantaneous power, freqs is
# the frequency vector, bins are the centers of the time bins in which
# the power is computed, and im is the matplotlib.image.AxesImage
# instance

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
NFFT = 512
duration = 1/float(framerate)*nframes  # расстойние между отсчетами
print duration

# теперь рисуем
# спектрограмма
ax1 = subplot(211)
Pxx, freqs, bins, im = specgram(
  channel,  # данные для построения
  NFFT = 128, # размер окна (STFT) 
  Fs = 44100,  # частота дискретизации
  Fc = 0,  # центерльная частота
  detrend=mlab.detrend_none,  # ?
  window=mlab.window_hanning,  # окно для преобразования
  noverlap = 32,  # на сколько перекрываются выборки
  pad_to = NFFT  # дополнение для ? увеличения разрешения
)
 
# временное представление
lench = len(channel)
xtime = np.ndarray(lench)
for i in range(lench):  # ось
  xtime[i] = (i*duration)/lench
  
subplot(212)  
plot(xtime, channel)   
plt.grid(True, color="g")
xs = 2048
axis([0, xtime[lench-1],-xs, xs])
show() 

"""
1. как узнать число элементов в массиве
2. подробнее про wav-блок
? 3. в спектре есть постоянная составляющаяя - странно это
- раздельное масштабирование осей
- поменять шаг сетки на степени кратные степень 2


"""
