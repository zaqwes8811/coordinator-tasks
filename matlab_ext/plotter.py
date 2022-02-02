# -*- coding: utf-8 -*-

from matplotlib.pyplot import plot, show, title, xlabel, ylabel
from matplotlib.pyplot import grid, figure

import matplotlib.pyplot as plt
from matplotlib.pyplot import show, plot, grid
from matplotlib.pyplot import legend

import numpy as np
#import cv2

def plot_dot( fs, zs, **args ):
    plot( fs, zs, 'bo', fillstyle='none', **args)

def close_all():
    plt.close('all')

def try_use_matplotlib( img ):   
    # fixme: bug - Matplot lib not working   
    old = cv2.imread('/home/zaqwes/hqXYw.jpg', cv2.CV_LOAD_IMAGE_GRAYSCALE )
    img[0:100, 0:100] = old[0:100, 0:100]

    plt.imshow(img, cmap='gray')
    plt.plot(100, 100, '-o')
    grid()
    plt.show()
    
def plot_point( img, p, style='o', size=5 ):
    if style == 'o':
        cv2.circle(img, p, size , 255, 1, 1)
        
    if style == 'r':
        p1 = ( int(p[0]+size), int(p[1]+size) )    
        cv2.rectangle( img, p, p1, 255, 1, 1 );

# fixme: Плохое API не удалось сделать ax=None по умолчанию, мешают прочие
#   аргументы.
def implot_ax(ax, x, y, *args, **kwargs):
    y = np.array( y )
    if ax:
        ax.plot(x, -y, *args, **kwargs)
    else:
        plot(x, -y, *args, **kwargs)
        
def implot( x, y, *args, **kwargs):
    y = np.array( y )
    plot(x, -y, *args, **kwargs)
    
def ocv2_plot():
    # неудобно для анализа
    #    cv2.imshow( 'img', img )
    ##    cv2.imshow( 'phy', img )
    #    
    #    # Нужно жать на окне
    #    while True:
    #        if cv2.waitKey(10) & 0xFF == ord('q'):
    #            break
    #    
    #    cv2.destroyAllWindows()
    pass

def cls():
    print "%c[2J" % (27)

if __name__=='__main__':
    # Simple data to display in various forms
    x = np.linspace(0, 2 * np.pi, 400)
    y = np.sin(x ** 2)
    
    plt.close('all')
    
    # Two subplots, unpack the axes array immediately
    f, (ax1, ax2) = plt.subplots(1, 2)#, sharey=True)
    implot( ax1, x, y )
    implot( ax1, x, y+100 )
    
    #ax1.set_title('Sharing Y axis')
    #ax2.scatter(x, y)
    
    plt.show()