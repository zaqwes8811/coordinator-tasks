# 0. install nvidia-docker2

# 1. ...

# https://elitedatascience.com/keras-tutorial-deep-learning-in-python
# sudo docker build -t ml_keras_tf_theano:latest .

# sudo docker start ml_keras_tf_theano_0
# sudo docker exec -it ml_keras_tf_theano_0 bash

# Stopped...
# https://hub.docker.com/r/eipdev/alpine-jupyter-notebook/~/dockerfile/
#FROM alpine:latest
# https://stackoverflow.com/questions/43840365/does-alpine-apk-have-an-ubuntu-apt-no-install-recommends-equivalent
# RUN apk update && apk add wget && rm -rf /var/cache/apk/*

# now Ubuntu
# https://blog.ubuntu.com/2018/07/09/minimal-ubuntu-released
# https://www.zdnet.com/article/minimal-ubuntu-for-containers-and-clouds/
# 18 now minimal
# https://github.com/tianon/docker-brew-ubuntu-core/tree/c7e9f7353aa24d1c35f501e06382aed1b540e85f
#FROM ubuntu:16.04  # нужна cuda
# Понадобилась cuda runtime
#FROM ubuntu:18.04
#FROM nvidia/cuda:7.5-devel

# FROM nvidia/cuda:7.5-cudnn5-runtime

#FROM nvidia/cuda:8.0-cudnn5-runtime
# но потом почему-то cudnn 7 доставляется
# не хочется тащить все
# FROM nvidia/cuda:7.5-cudnn5-devel

#FROM nvidia/cuda:9.0-cudnn7-runtime
#FROM nvidia/cuda:8.0-cudnn5-runtime
# Theano then
FROM nvidia/cuda:7.5-cudnn5-devel


LABEL authors="Igor Lugansky <igor.a.lugansky@gmail.com>, Golovchanskaia Iuiia <golju@bk.ru>"

RUN apt-get update && apt-get install -y \
	build-essential \
    git-core wget \
    nano \
    curl \
    python-dev \
 	&& rm -rf /var/lib/apt/lists/* 

# http://pythonwise.blogspot.com/2015/04/docker-miniconda-perfect-match.html
# wget https://repo.continuum.io/miniconda/Miniconda2-latest-Linux-x86_64.sh
# задает вопросы
RUN curl -LO https://repo.continuum.io/miniconda/Miniconda2-latest-Linux-x86_64.sh \
	&& bash Miniconda2-latest-Linux-x86_64.sh -p /root/miniconda2 -b \
	&& rm Miniconda2-latest-Linux-x86_64.sh

# Split
RUN /root/miniconda2/bin/pip install cython \
	&& /root/miniconda2/bin/conda update -y conda \
	&& /root/miniconda2/bin/conda install scipy nose sphinx pydot-ng \
	matplotlib jupyterlab ipywidgets mkl numpy \
	&& /root/miniconda2/bin/conda clean -tisy



#  - no if runtime then cuda.h not found
# https://codeyarns.com/2015/07/31/pip-install-error-with-pycuda/
# 
RUN /root/miniconda2/bin/conda install  theano pygpu \
	&& /root/miniconda2/bin/conda clean -tisy \
	&& /root/miniconda2/bin/pip install scikit-cuda pycuda


# # conda clean????
# # can't install /root/miniconda2/bin/pip install --upgrade matplotlib jupyterlab ipywidgets
# # tipsy -> tisy
# # keras поставить tf
# RUN /root/miniconda2/bin/conda install tensorflow-gpu==1.10.0 \
# 	&& /root/miniconda2/bin/conda clean -tisy

# CPU only
RUN /root/miniconda2/bin/conda install tensorflow==1.10.0 \
 	&& /root/miniconda2/bin/conda clean -tisy

RUN /root/miniconda2/bin/conda install keras=2.0.2 \
	&& /root/miniconda2/bin/conda clean -tisy

# FIXME: remove useless files

RUN wget https://cmake.org/files/v3.10/cmake-3.10.0.tar.gz \
	&& tar -xvf cmake-3.10.0.tar.gz \
	&& cd cmake-3.10.0 && ./configure && make -j4 && make install && cd ..

# for opencv
RUN apt-get update && apt-get install -y \
	libtbb-dev \
 	&& rm -rf /var/lib/apt/lists/* 

# opencvmake
# python is special here
#-D PYTHON_LIBRARY=/root/miniconda2/lib/python2.7/config/libpython2.7.a 
RUN wget https://github.com/opencv/opencv/archive/2.4.2.tar.gz \
	&& tar -xvf 2.4.2.tar.gz && cd opencv-2.4.2 && mkdir build && cd build \
	&& cmake \
		-DCMAKE_INSTALL_PREFIX="/opt/opencv-2.4.2" \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_SHARED_LIBS=ON \
		-DBUILD_WITH_STATIC_CRT=OFF \
		-DBUILD_DOCS=OFF \
		-DBUILD_EXAMPLES=OFF \
		-DBUILD_PACKAGE=OFF \
		-DBUILD_TESTS=OFF \
		-DBUILD_PERF_TESTS=OFF \
		-DBUILD_WITH_DEBUG_INFO=OFF \
		-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=OFF \
		-DCMAKE_SKIP_RPATH=OFF \
		-DCMAKE_USE_RELATIVE_PATHS=OFF \
		-DENABLE_FAST_MATH=ON \
		-DENABLE_NOISY_WARNINGS=OFF \
		-DENABLE_OMIT_FRAME_POINTER=ON \
		-DENABLE_PRECOMPILED_HEADERS=OFF \
		-DENABLE_PROFILING=OFF \
		-DENABLE_SOLUTION_FOLDERS=OFF \
		-DOPENCV_CAN_BREAK_BINARY_COMPATIBILITY=ON \
		-DBUILD_opencv_core=ON \
		-DBUILD_opencv_highgui=ON \
		-DBUILD_opencv_imgproc=ON \
		-DBUILD_opencv_ml=ON \
		-DBUILD_opencv_python=ON \
		-D PYTHON_EXECUTABLE=/root/miniconda2/bin/python \
		-D PYTHON_INCLUDE_DIR=/root/miniconda2/include/python2.7 \
		-DBUILD_opencv_ts=OFF \
		-DENABLE_POWERPC=OFF \
		-DENABLE_SSE=OFF \
		-DENABLE_SSE2=OFF \
		-DENABLE_SSE3=OFF \
		-DENABLE_SSSE3=OFF \
		-DBUILD_JASPER=OFF \
		-DBUILD_JPEG=OFF \
		-DBUILD_PNG=OFF \
		-DBUILD_TIFF=OFF \
		-DBUILD_ZLIB=OFF \
		-DBUILD_ANDROID_CAMERA_WRAPPER=OFF \
		-DBUILD_ANDROID_EXAMPLES=OFF \
		-DBUILD_FAT_JAVA_LIB=OFF \
		-DBUILD_JAVA_SUPPORT=OFF \
		-DBUILD_NEW_PYTHON_SUPPORT=ON \
		-DBUILD_PYTHON_SUPPORT=ON \
		-DINSTALL_ANDROID_EXAMPLES=OFF \
		-DINSTALL_C_EXAMPLES=OFF \
		-DINSTALL_PYTHON_EXAMPLES=ON \
		-DINSTALL_TO_MANGLED_PATHS=OFF \
		-DWITH_1394=OFF \
		-DWITH_ANDROID_CAMERA=OFF \
		-DWITH_AVFOUNDATION=OFF \
		-DWITH_CARBON=OFF \
		-DWITH_CUBLAS=OFF \
		-DWITH_CUDA=OFF \
		-DWITH_CUFFT=OFF \
		-DWITH_EIGEN=OFF \
		-DWITH_IMAGEIO=OFF \
		-DWITH_IPP=OFF \
		-DWITH_JASPER=OFF \
		-DWITH_OPENEXR=OFF \
		-DWITH_OPENGL=OFF \
		-DWITH_OPENNI=OFF \
		-DWITH_PVAPI=OFF \
		-DWITH_QUICKTIME=OFF \
		-DWITH_TBB=ON \
		-DWITH_UNICAP=OFF \
		-DWITH_VIDEOINPUT=OFF \
		-DWITH_XIMEA=OFF \
		-DWITH_XINE=OFF \
		-DWITH_FFMPEG=OFF \
		-DWITH_GSTREAMER=OFF \
		-DWITH_GTK=OFF \
		-DWITH_JPEG=ON \
		-DWITH_PNG=ON \
		-DWITH_QT=OFF \
		-DWITH_TIFF=OFF \
		-DWITH_V4L=OFF .. \
		&& make -j9 && make install

#################################

# http://caffe.berkeleyvision.org/install_apt.html
# https://stackoverflow.com/questions/46253744/getting-error-attributeerror-module-object-has-no-attribute-ifelse
RUN /root/miniconda2/bin/conda install keras \
	&& /root/miniconda2/bin/conda clean -tisy

# mkl
# https://gist.github.com/pachamaltese/afc4faef2f191b533556f261a46b3aa8

RUN apt-get update && apt-get install -y \
	libblas-dev liblapack-dev gfortran \
	libatlas-base-dev libopenblas-dev \
 	&& rm -rf /var/lib/apt/lists/* 

RUN /root/miniconda2/bin/conda install scikit-learn \
	&& /root/miniconda2/bin/conda clean -tisy

#BLAS: install ATLAS by sudo apt-get install libatlas-base-dev or install OpenBLAS by sudo apt-get install libopenblas-dev or MKL for better CPU performance.
#sudo apt-get install libatlas-base-dev or install OpenBLAS by sudo apt-get install libopenblas-dev

#theano:
#https://stackoverflow.com/questions/21608025/how-to-set-up-theano-config
# https://www.asozykin.ru/deep_learning/2017/03/11/How-to-use-gpu-with-theano.html
# https://github.com/Theano/Theano/wiki/Converting-to-the-new-gpu-back-end%28gpuarray%29
# http://queirozf.com/entries/setup-keras-theano-backend-and-gpu-on-ubuntu-16-04

# GpuArrayException: GPU is too old for CUDA version
# THEANO_FLAGS=mode=FAST_RUN,device=cuda0,floatX=float32 /root/miniconda2/bin/python2.7 TheanoGPU.py
# DEVICE=cuda0 python -c "import pygpu;pygpu.test()"
# https://stackoverflow.com/questions/47597499/gpuarrayexception-gpu-is-too-old-for-cuda-version

# Настраиваем CNMeM

# libgpuarray
# http://deeplearning.net/software/libgpuarray/installation.html



# https://stackoverflow.com/questions/42177658/how-to-switch-backend-with-keras-from-tensorflow-to-theano
# mkdir .keras
# touch ./keras/keras.json
# 

COPY keras.json /root/.keras/
COPY theanorc /root/.theanorc 

# jupiter

# launch
# here will be jypiter
COPY main.sh /sbin
RUN chmod +x /sbin/main.sh
CMD ["/sbin/main.sh"]

#/root/miniconda2/bin/jupyter-lab --ip=0.0.0.0 --port=8888 --allow-root
# PYTHONPATH="`pwd`:/opt/opencv-2.4.2/lib/python2.7/site-packages" /root/miniconda2/bin/jupyter-notebook --ip=0.0.0.0 --port=8888 --allow-root

#WORKDIR /mnt/notebook
#ENTRYPOINT ["/sbin/tini", "--"]
#CMD ["jupyter", "lab", "--ip=0.0.0.0", "--port=8888", "--no-browser", "--allow-root"]

# Troubles:
# https://github.com/Theano/Theano/issues/6533
# http://deeplearning.net/software/theano/troubleshooting.html
# https://github.com/ChicagoBoothAnalytics/Software/blob/master/AWS/EMR/.config/.theanorc
# ~/.theanorc 


# /root/miniconda2/bin/tensorboard  --logdir /Graph/


# DANGER:
# https://github.com/tensorflow/tensorflow/issues/16898

