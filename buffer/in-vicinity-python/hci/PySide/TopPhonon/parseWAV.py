#!/bin/python
""" 
  file : import parseWAV
	
  Authors:
    LI Ligansky Igor 

  Changes:
    1-Nov-11 : создан  // надо ли писать
"""
import wave
import numpy as np


types = {  #  numpy
    1: np.int8,
    2: np.int16,
    4: np.int32
}

# выборка пареметров из файла
class parseWAVFile:
  # Property
  _fName = None  #
  _fNameOut = None 
  _nchannels = None
  _sampwidth = None # разрядность данных
  _framerate = None # частота дискретизации
  _nframes = None # число фрэймов
  _comptype = None
  _compname = None
  def parseCaption(self):
    wav = wave.open(self._fName, mode="r")
    ( 
      self._nchannels,  self._sampwidth,  
      self._framerate,  self._nframes,  
      self._comptype, self._compname
    ) = wav.getparams()
    return ['asdf', self._nchannels, self._sampwidth, self._framerate, self._nframes]
  def getFileName(self, fName):
    self._fName = fName

# создаем экземпляр
pw = parseWAVFile()