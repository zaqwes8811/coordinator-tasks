# fixme: add rm

# git clone https://github.com/openexr/openexr.git

wget https://github.com/openexr/openexr/archive/v2.2.0.tar.gz
tar -xvf v2.2.0.tar.gz

cd openexr-2.2.0
cd IlmBase
mkdir build
cd build
# I...base
cmake -DCMAKE_INSTALL_PREFIX=/opt/lib/openexr ..

cd ../../OpenEXR

# op....
mkdir build && cd build 
#&& \
cmake -DCMAKE_INSTALL_PREFIX=/opt/lib/openexr -DILMBASE_PACKAGE_PREFIX=/opt/lib/openexr  ..

sudo ln -s /usr/include/OpenEXR/ /usr/local/include/


##########################

git clone https://github.com/Blosc/c-blosc.git
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
tar -xvf v5.0.0.tar.gz
cd openvdb-5.0.0/
cd openvdb


make -j9 install \
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

###############################################################


########################### openvdb

sudo apt-get install openexr
sudo apt-get install liblog4cplus-dev
sudo apt-get install libcppunit-dev

sudo ln -s /usr/include/OpenEXR/ /usr/local/include/

cmake -D BLOSC_LOCATION=/usr/local -D TBB_LOCATION=/usr/local \
    -D ILMBASE_LOCATION=/usr/local -D OPENEXR_LOCATION=/usr \
    -DCPPUNIT_LOCATION=/usr/local \
    -DOpenVDB_LIBRARY_DIR=/tmp ..

http://kirilllykov.github.io/blog/2013/02/04/openvdb-installation-on-macos/

https://groups.google.com/forum/#!topic/openvdb-forum/r6Q1pRsKhi8
maybe better
https://wiki.blender.org/index.php/Dev:Doc/Building_Blender/Linux/Dependencies_From_Source