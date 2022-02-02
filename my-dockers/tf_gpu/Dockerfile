# docker build -t tf_gpu:latest .
#
# https://blog.codeship.com/the-basics-of-the-docker-run-command/
# sudo nvidia-docker run -v /mnt:/mnt -p 8888:8888 -p 6006:6006 --name tf_gpu_0 --tmpfs /tmp tf_gpu:latest 

# fixme: from tf
#FROM nvidia/cuda:7.5-devel
FROM tensorflow/tensorflow:latest-gpu

# похоже нужно было прокинуть порты
# https://stackoverflow.com/questions/33636925/how-do-i-start-tensorflow-docker-jupyter-notebook

# "TERM environment variable not set." ???

RUN apt-get update && apt-get install -y \
    git-core wget \
	libjemalloc-dev \
	libtbb-dev \
	libboost-all-dev \
 	&& rm -rf /var/lib/apt/lists/* 

# launch
# here will be jypiter
#COPY main.sh /usr/local/bin
#RUN chmod +x /usr/local/bin/main.sh