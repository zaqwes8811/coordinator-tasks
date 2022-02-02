#!/bin/bash -x

sudo docker stop yolo3_4_py_0 && sudo docker rm yolo3_4_py_0
sudo docker run \
	-v /mnt:/mnt \
	-p 8889:8888 -p 6007:6006 \
	--name yolo3_4_py_0 \
	--tmpfs /tmp yolo3_4_py:latest 
