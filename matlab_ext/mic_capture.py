#!/usr/bin/env python
# coding: utf-8

import sys
sys.path.append( ".." )  # fixme: bad!
#from . import *  # not working
from matlab_ext import *

#http://python-sounddevice.readthedocs.org/en/0.3.1/
# sudo apt-get install libffi-dev
# sudo pip install cffi
# sudo pip install sounddevice

import sounddevice as sd
import numpy as np

# fixme: записать в *.wav

#
fs = 1000#48000

sd.default.samplerate = fs
sd.default.dtype = 'int16'  # fixme: uint?
sd.default.channels = 1

duration = 15
X = sd.rec(duration * fs)
X = (X.T)[0]

sd.wait()
# Fs = fs            #% Sampling frequency
# T = 1./Fs             #% Sampling period
# L = 1000             #% Length of signal

# t = colon(0, L-1)*T        #% Time vector
# X = 0.7*sin(2*pi*50*t) + sin(2*pi*120*t)

P1, f = fft_one_side( X , size( X ), fs )
#plot( X )
plot( f, P1 )
grid()
show()
