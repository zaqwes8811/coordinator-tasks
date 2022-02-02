# coding: utf-8

from numpy.random import randn

from numpy.fft import fft

from numpy import sin, pi, arange
#, abs

import numpy

# matlab linspace
# http://www.mathworks.com/help/matlab/ref/linspace.html
#
# matlab colon
# http://www.mathworks.com/help/matlab/ref/colon.html
# j:k for ints [j,... k]
# [j,j+1,j+2,...,j+m], where m = fix(k-j)
def colon( j, k ):
	return arange(j, k+1)

def size( a ):
	return len( a )

def fft_one_side( X, L, Fs ):
	""" В логическом виде """
	Y = fft( X )
	P2 = numpy.abs( Y/L )
	P1 = P2[1-1:L/2+1-1]
	P1[2-1:-1-1] = 2*P1[2-1:-1-1]

	f = Fs*colon(0, L/2-1) / L
	return P1, f
