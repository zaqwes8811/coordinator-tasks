#!/usr/bin/env python
# coding: utf-8

# PYTHONPATH=$PYTHONPATH:..

# Numpy:
#   http://web.mit.edu/dvp/Public/numpybook.pdf

from control.matlab import bode
from control.matlab import series
import numpy as np

import matplotlib.pyplot as plt

def grid():
	plt.grid()

if __name__ == '__main__':
	
	w = np.logspace(-1, 2)
	# log scale of angular
	# frequency w
	num = 5000
	den = [1, 55, 250, 0]
	# bode(num, den, w)

	print np.convolve([1,4,0,2], [1,0,0,3,1])