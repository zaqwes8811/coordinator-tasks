#!/usr/bin/env python
# coding: utf-8

import time

import numpy as np
import cv2

cap = cv2.VideoCapture('../Documents/sample.avi')

rois = []

frame_counter = 0
while cap.isOpened():
    ret, frame = cap.read()
    #if not ret:
   	#	break

    frame_counter += 1
    #If the last frame is reached, reset the capture and the frame_counter
    if frame_counter == cap.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT):
        frame_counter = 0 #Or whatever as long as it is the same as next line
        cap.set(cv2.cv.CV_CAP_PROP_POS_FRAMES, 0)

    gray = cv2.cvtColor( frame, cv2.COLOR_BGR2GRAY )

    # add rects
    #print img.shape
    # fixme: присваивание очень медленное! numpy
    img = gray / 10

    # fixme: как бы обернуть то
    #print 0:20
    # numpy.take(...)
    img[ 0:20, 1:10 ] = 220
    # break


    # draw
    cv2.circle( img, (100,150), 10, 255 )

    font = cv2.FONT_HERSHEY_SIMPLEX
    text = 'marker:' + str( frame_counter )
    cv2.putText( img, text, (5,200), font, 0.4, (225),1 )

    # processing
    blur = img#cv2.GaussianBlur( img,(5,5), 0 )

    cv2.imshow( 'frame', blur )

    # Нужно жать на окне
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()