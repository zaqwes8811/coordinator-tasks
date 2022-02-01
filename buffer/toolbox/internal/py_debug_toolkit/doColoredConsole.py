#-*- coding: utf-8 -*-
# file : import doColoredConsole as doColor
import ctypes
import sys
FOREGROUND_BLUE = 0x01 # text color contains blue.
FOREGROUND_GREEN= 0x02 # text color contains green.
FOREGROUND_RED  = 0x04 # text color contains red.
FOREGROUND_INTENSITY = 0x08 # text color is intensified.
BACKGROUND_BLUE = 0x10 # background color contains blue.
BACKGROUND_GREEN= 0x20 # background color contains green.
BACKGROUND_RED  = 0x40 # background color contains red.
BACKGROUND_INTENSITY = 0x80 # background color is intensified.
NORMAL  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
WHITE   = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
RED     = FOREGROUND_RED|FOREGROUND_INTENSITY;
DKRED     = FOREGROUND_RED;
BLUE    = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
DKBLUE    = FOREGROUND_BLUE|FOREGROUND_GREEN;
GREEN   = FOREGROUND_GREEN|FOREGROUND_INTENSITY;
DKGREEN   = FOREGROUND_GREEN;
YELLOW  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
BROWN   = FOREGROUND_RED|FOREGROUND_GREEN;

STD_INPUT_HANDLE = -10
#STD_OUTPUT_HANDLE= -11
STD_ERROR_HANDLE = -12

class ColoredOut:
	_handle = None
	_reset = None
	# как бы конструктор
	def __init__ ( self ):
		self._initColoredConsoleOut()
	# процедура закрытия вывода
	def _setReset( self ):
		# Based on IPython's winconsole.py, written by Alexander Belchenko
		import struct
		csbi = ctypes.create_string_buffer(22)
		res = ctypes.windll.kernel32.GetConsoleScreenBufferInfo( self._handle, csbi)
		#assert res

		(bufx, bufy, curx, cury, wattr,
		left, top, right, bottom, maxx, maxy) = struct.unpack("hhhhHhhhhhh", csbi.raw)
		self._reset = wattr

	def _initColoredConsoleOut( self ):
		# Constants from the Windows API
		STD_OUTPUT_HANDLE = -11
		self._handle = ctypes.windll.kernel32.GetStdHandle(STD_OUTPUT_HANDLE)
		self._setReset( )

	''' Error '''
	def printE( self, msg ):
		colorType    = RED # text color contains red.
		self._printMsg( msg, colorType )
	
	''' Warning'''
	def printW( self, msg ):
		colorType    = YELLOW # text color contains red.
		self._printMsg( msg, colorType )
		
	def printN( self, msg ):
		colorType    = NORMAL # text color contains red.
		self._printMsg( msg, colorType )

	def _printMsg( self, msg, colorType ):
		ctypes.windll.kernel32.SetConsoleTextAttribute( self._handle, colorType)
		#print( msg, end='')
		#sys.stdout.write( msg )
		print msg,
		ctypes.windll.kernel32.SetConsoleTextAttribute( self._handle, self._reset)
		

co = ColoredOut()