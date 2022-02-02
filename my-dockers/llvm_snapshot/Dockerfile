
# docker build -t llvm_snapshot:latest .
FROM ubuntu:17.10

# pkgs
RUN apt-get update && apt-get install -y \
    build-essential \
    libsqlite3-dev \
    git-core wget subversion wget \
	libjemalloc-dev \
	libtbb-dev \
	libboost-all-dev \
 	&& rm -rf /var/lib/apt/lists/* 

# cmake
WORKDIR /home
RUN wget https://cmake.org/files/v3.10/cmake-3.10.0.tar.gz \
	&& tar -xvf cmake-3.10.0.tar.gz \
	&& cd cmake-3.10.0 && ./configure && make -j4 && make install && cd ..