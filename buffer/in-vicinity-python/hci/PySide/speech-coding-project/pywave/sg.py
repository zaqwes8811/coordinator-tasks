#!/usr/bin/env python
# -*- coding: utf-8 -*-
from pylab import *

import wave
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker  # �������������� ��������� �����
import math

types = {  # �� numpy
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
  sampwidth,  # ����������� ������
  framerate,  # ������� �������������
  nframes,  # ����� �������� 
  comptype, compname
) = wav.getparams()

print  nchannels, sampwidth, framerate, nframes
content = wav.readframes(nframes)  # ������ ����� ���
samples = np.fromstring(content, dtype=types[sampwidth])  # ������������ ������. types
channel = samples[0::nchannels]  # �������� �� �������
NFFT = 512
duration = 1/float(framerate)*nframes  # ���������� ����� ���������
print duration

# ������ ������
# �������������
ax1 = subplot(211)
Pxx, freqs, bins, im = specgram(
  channel,  # ������ ��� ����������
  NFFT = 128, # ������ ���� (STFT) 
  Fs = 44100,  # ������� �������������
  Fc = 0,  # ����������� �������
  detrend=mlab.detrend_none,  # ?
  window=mlab.window_hanning,  # ���� ��� ��������������
  noverlap = 32,  # �� ������� ������������� �������
  pad_to = NFFT  # ���������� ��� ? ���������� ����������
)
 
# ��������� �������������
lench = len(channel)
xtime = np.ndarray(lench)
for i in range(lench):  # ���
  xtime[i] = (i*duration)/lench
  
subplot(212)  
plot(xtime, channel)   
plt.grid(True, color="g")
xs = 2048
axis([0, xtime[lench-1],-xs, xs])
show() 

"""
1. ��� ������ ����� ��������� � �������
2. ��������� ��� wav-����
? 3. � ������� ���� ���������� ������������� - ������� ���
- ���������� ��������������� ����
- �������� ��� ����� �� ������� ������� ������� 2


"""
