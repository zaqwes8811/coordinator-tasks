
# Best practices
# https://docs.docker.com/engine/userguide/eng-image/dockerfile_best-practices/#run

# docker build -t 3d:latest .
#
# https://blog.codeship.com/the-basics-of-the-docker-run-command/
# sudo nvidia-docker run --name 3d_0 --tmpfs /tmp 3d:latest 
FROM nvidia/cuda:7.5-devel

# base
# liblog4cplus-dev
RUN apt-get update && apt-get install -y \
    build-essential \
    libsqlite3-dev \
    git-core wget \
    openexr \
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

# fixme: add rm

git clone https://github.com/openexr/openexr.git
# I...base
cmake -DCMAKE_INSTALL_PREFIX=/opt/lib/openexr ..
# ...
cmake -DCMAKE_INSTALL_PREFIX=/opt/lib/openexr -DILMBASE_PACKAGE_PREFIX=/opt/lib/openexr  ..

#wget https://github.com/openexr/openexr/releases/download/v2.2.1/openexr-2.2.1.tar.gz
#export CXXFLAGS=-isystem\ /opt/lib/openexr/OpenEXR/include
#libtool ??? it was warning

mkdir build
cd build/
 
# set up install dir
cmake -DCMAKE_INSTALL_PREFIX=/opt/lib/blosc/ -DBUILD_TESTS=false ..


 
# compile and install
cmake --build . --target install

# OpenVDB
# https://wiki.blender.org/index.php/User:Kevindietrich/OpenVDBCompile
# https://wiki.blender.org/index.php/Dev:Doc/Building_Blender/Linux/Dependencies_From_Source
# from src
# https://github.com/openexr/openexr

wget https://github.com/dreamworksanimation/openvdb/archive/v5.0.0.tar.gz

COPY main.sh /usr/local/bin
RUN chmod +x /usr/local/bin/main.sh

tree



make -j4 install \
    DESTDIR="/opt/lib/openvdb" \
    EXR_INCL_DIR="/opt/lib/openexr/include" \
    EXR_LIB_DIR="/opt/lib/openexr/lib" \
    EXR_LIB="-lIlmImf-2_2" \
    BOOST_INCL_DIR="/usr/include" \
    BOOST_LIB_DIR="/usr/lib" \
    BOOST_LIB="-lboost_iostreams -lboost_system" \
    BOOST_THREAD_LIB="-lboost_thread" \
    ILMBASE_INCL_DIR="/opt/lib/openexr/include" \
    ILMBASE_LIB_DIR="/opt/lib/openexr/lib" \
    ILMBASE_LIB="-lIlmThread-2_2 -lIex-2_2 -lImath-2_2" \
    HALF_LIB="-lHalf" \
    TBB_INCL_DIR="/usr/include" \
    TBB_LIB_DIR="/usr/lib" \
    TBB_LIB="-ltbb" \
    BLOSC_INCL_DIR="/opt/lib/blosc/include" \
    BLOSC_LIB_DIR="/opt/lib/blosc/lib" \
    BLOSC_LIB="-lblosc -lz" \
    CONCURRENT_MALLOC_LIB="-ljemalloc" \
    CONCURRENT_MALLOC_LIB_DIR="/usr/lib/x86_64-linux-gnu" \
    CPPUNIT_INCL_DIR="" \
    CPPUNIT_LIB_DIR="" \
    CPPUNIT_LIB="" \
    LOG4CPLUS_INCL_DIR="" \
    LOG4CPLUS_LIB_DIR="" \
    LOG4CPLUS_LIB="" \
    GLFW_INCL_DIR="" \
    GLFW_LIB_DIR="" \
    GLFW_LIB="" \
    GLFW_MAJOR_VERSION="" \
    PYTHON_VERSION="" \
    PYTHON_INCL_DIR="" \
    PYCONFIG_INCL_DIR="" \
    PYTHON_LIB_DIR="" \
    PYTHON_LIB="" \
    BOOST_PYTHON_LIB_DIR="" \
    BOOST_PYTHON_LIB="" \
    NUMPY_INCL_DIR="" \
    EPYDOC="" \
    PYTHON_WRAP_ALL_GRID_TYPES="no" \
    DOXYGEN=""


# что-то ничего не сработало!!!
# https://www.ctl.io/developers/blog/post/gracefully-stopping-docker-containers/
#FROM ubuntu:trusty
#COPY loop.sh /
#CMD /loop.sh
# prevent out
#ENTRYPOINT ["tail", "-f", "/dev/null"]
ENTRYPOINT ["/usr/local/bin/main.sh"]