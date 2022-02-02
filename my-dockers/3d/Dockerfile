
# Best practices
# https://docs.docker.com/engine/userguide/eng-image/dockerfile_best-practices/#run

# docker build -t 3d:latest .
#
# https://blog.codeship.com/the-basics-of-the-docker-run-command/
# sudo nvidia-docker run -v /mnt:/mnt --name 3d_0 --tmpfs /tmp 3d:latest 
FROM nvidia/cuda:7.5-devel

# base
# liblog4cplus-dev
#    openexr 
RUN apt-get update && apt-get install -y \
    build-essential \
    libsqlite3-dev \
    git-core wget \
	libjemalloc-dev \
	libtbb-dev \
	libboost-all-dev \
 	&& rm -rf /var/lib/apt/lists/* 

# not needed && apt-get clean

# Cmake need last
# 
#WORKDIR /tmp  # ./configure not working
WORKDIR /home
RUN wget https://cmake.org/files/v3.10/cmake-3.10.0.tar.gz \
	&& tar -xvf cmake-3.10.0.tar.gz \
	&& cd cmake-3.10.0 && ./configure && make -j4 && make install && cd ..

# что-то ничего не сработало!!!
# https://www.ctl.io/developers/blog/post/gracefully-stopping-docker-containers/
#FROM ubuntu:trusty
#COPY loop.sh /
#CMD /loop.sh
# prevent out
#ENTRYPOINT ["tail", "-f", "/dev/null"]
# COPY main.sh /usr/local/bin
# RUN chmod +x /usr/local/bin/main.sh
# ENTRYPOINT ["/usr/local/bin/main.sh"]

# start runsv 
WORKDIR /tmp
# COPY runsvdir-start /sbin
COPY main.sh /sbin/
# COPY init.sh /home/
# COPY default_calib.sh /home/
RUN chmod +x /sbin/main.sh 
# && ln -s /etc/service /service \
#     && chmod +x /home/init.sh && chmod +x /home/default_calib.sh

WORKDIR /home
# int main() {}
# https://docs.docker.com/engine/admin/multi-service_container/
# Start all
CMD ["/sbin/main.sh"]
