https://askubuntu.com/questions/913241/error-in-executing-opencv-in-ubuntu

sudo apt install libgtk2.0-dev pkg-config

cmake -DCMAKE_INSTALL_PREFIX="/opt/opencv-3.4.3" \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_DOCS=OFF \
		-DBUILD_EXAMPLES=OFF \
		-DWITH_IPP=ON -DWITH_QT=OFF -DWITH_TBB=ON -DENABLE_PRECOMPILED_HEADERS=OFF \
		-DWITH_GTK_2_X=ON \
		-DPYTHON_EXECUTABLE=/root/miniconda2/bin/python \
		-DPYTHON_INCLUDE_DIR=/root/miniconda2/include/python2.7 \
		..

Speed up:
https://github.com/pjreddie/darknet/issues/80
https://groups.google.com/forum/#!topic/darknet/M921U9rxzPI

# sudo docker build -t yolo3_4_py:latest .

#sudo docker stop yolo3_4_py_0 && docker rm yolo3_4_py_0
#sudo docker run --name yolo3_4_py_0 --tmpfs /tmp yolo3_4_py:latest 

# sudo docker start yolo3_4_py_0
# sudo docker exec -it yolo3_4_py_0 bash

# https://www.kaggle.com/sajinpgupta/object-detection-using-yolov3

# Very long running - gpu
# nothing found

FROM ubuntu:16.04

RUN apt-get update && apt-get install -y \
	python3.5 \
	python3-pip \
	git \
	build-essential cmake \
	qt5-default libvtk6-dev \
	zlib1g-dev libjpeg-dev libwebp-dev libpng-dev libtiff5-dev libjasper-dev libopenexr-dev libgdal-dev \
	libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev libtheora-dev libvorbis-dev \
	libxvidcore-dev libx264-dev yasm libopencore-amrnb-dev libopencore-amrwb-dev libv4l-dev libxine2-dev \
	python-dev python-tk python-numpy python3-dev python3-tk python3-numpy \
	unzip wget \
	&& rm -rf /var/lib/apt/lists/* 

RUN wget https://github.com/opencv/opencv/archive/3.4.0.zip && unzip 3.4.0.zip \
	&& rm 3.4.0.zip && cd /opencv-3.4.0 && mkdir build && cd /opencv-3.4.0/build && cmake -DBUILD_EXAMPLES=OFF .. \
	&& make -j9 && make install && ldconfig

## Downloading and compiling darknet ##
WORKDIR /
RUN git clone https://github.com/pjreddie/darknet.git
WORKDIR /darknet
# Set OpenCV makefile flag
RUN sed -i '/OPENCV=0/c\OPENCV=1' Makefile
RUN make
ENV DARKNET_HOME /darknet
ENV LD_LIBRARY_PATH /darknet

## Download and compile YOLO3-4-Py ##
WORKDIR /
RUN git clone https://github.com/madhawav/YOLO3-4-Py.git
WORKDIR /YOLO3-4-Py
RUN pip3 install pkgconfig cython
RUN python3 setup.py build_ext --inplace

## Run test ##
RUN sh download_models.sh
ADD ./docker_demo.py /YOLO3-4-Py/docker_demo.py
#CMD ["python3", "docker_demo.py"]

# https://pjreddie.com/darknet/yolo/
# https://github.com/tzutalin/labelImg

# here will be jypiter
COPY main.sh /sbin
RUN chmod +x /sbin/main.sh
CMD ["/sbin/main.sh"]